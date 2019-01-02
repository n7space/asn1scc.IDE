/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "document.h"

#include <QTextBlock>

#include <coreplugin/idocument.h>

#include <projectexplorer/project.h>
#include <projectexplorer/session.h>

#include <texteditor/fontsettings.h>
#include <texteditor/texteditorsettings.h>

#include "projectcontenthandler.h"

using namespace Asn1Acn::Internal;

static const int PROCESS_DOCUMENT_INTERVAL_MS = 1000;

Document::Document()
{
    m_processorTimer.setSingleShot(true);
    m_processorTimer.setInterval(PROCESS_DOCUMENT_INTERVAL_MS);

    connect(&m_processorTimer,
            &QTimer::timeout,
            this,
            &Document::processDocument,
            Qt::UniqueConnection);

    connect(this, &Core::IDocument::filePathChanged, this, &Document::onFilePathChanged);
}

void Document::scheduleProcessDocument()
{
    m_processorTimer.start();
}

void Document::onFilePathChanged(const Utils::FileName &oldPath, const Utils::FileName &newPath)
{
    if (newPath.isEmpty() || newPath == oldPath)
        return;

    updateExtraSelections();

    connect(this, &Core::IDocument::contentsChanged, this, &Document::onFileContentChanged);

    connect(ModelValidityGuard::instance(),
            &ModelValidityGuard::modelChanged,
            this,
            &Document::onModelChanged);
}

void Document::onModelChanged()
{
    updateExtraSelections();
}

void Document::onFileContentChanged()
{
    auto file = ParsedDataStorage::instance()->getAnyFileForPath(filePath());
    if (file == nullptr)
        return;

    file->setPolluted();
    file->clearReferences();

    scheduleProcessDocument();
}

namespace {
QList<QTextEdit::ExtraSelection> errorsToSelection(
    const std::vector<Data::ErrorMessage> &errorMessages, QTextDocument *textDocument)
{
    const auto errorFormat = TextEditor::TextEditorSettings::instance()
                                 ->fontSettings()
                                 .toTextCharFormat(TextEditor::C_ERROR);

    QList<QTextEdit::ExtraSelection> result;

    for (const auto &message : errorMessages) {
        const auto &location = message.location();

        QTextEdit::ExtraSelection selection;
        selection.format = errorFormat;
        selection.format.setToolTip(message.message());

        QTextCursor cursor(textDocument->findBlockByNumber(location.line() - 1));
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, location.column());
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        selection.cursor = cursor;

        result.append(selection);
    }

    return result;
}
} // namespace

void Document::processDocument()
{
    ProjectContentHandler *handler = ProjectContentHandler::create();
    handler->handleFileContentChanged(filePath());
}

void Document::updateExtraSelections() const
{
    const auto file = ParsedDataStorage::instance()->getFileForPathFromProject(activeProjectName(),
                                                                               filePath());
    if (file == nullptr)
        return;

    const auto list = errorsToSelection(file->errors(), document());

    emit extraSelectionsUpdated(list);
}

const QString Document::activeProjectName() const
{
    const auto project = ProjectExplorer::SessionManager::startupProject();
    if (project == nullptr)
        return QString();

    return project->displayName();
}

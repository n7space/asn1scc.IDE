/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.pl/Space
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
#include "indexupdater.h"

#include <texteditor/textdocument.h>

#include "model.h"

using namespace Asn1Acn::Internal::TreeViews;

static const int UPDATE_INTERVAL_MS = 500;

IndexUpdater::IndexUpdater(const Model *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_editorWidget(nullptr)
{
    createUpdateTimer();
}

void IndexUpdater::setEditor(TextEditor::TextEditorWidget *editorWidget)
{
    unsetEditor();

    m_editorWidget = editorWidget;

    if (m_editorWidget != nullptr) {
        connect(m_editorWidget, &QPlainTextEdit::cursorPositionChanged,
                m_updateIndexTimer, static_cast<void (QTimer::*)()>(&QTimer::start));
        m_updateIndexTimer->start();
    } else {
        emit currentIndexUpdated(QModelIndex());
    }
}

void IndexUpdater::unsetEditor()
{
    if (m_editorWidget != nullptr)
        m_editorWidget->disconnect(m_updateIndexTimer);

    m_editorWidget = nullptr;
}

void IndexUpdater::updateCurrentIndex()
{
    updateNow();
}

void IndexUpdater::createUpdateTimer()
{
    m_updateIndexTimer = new QTimer(this);
    m_updateIndexTimer->setObjectName("OverviewIndexUpdater::UpdateTimer");
    m_updateIndexTimer->setSingleShot(true);
    m_updateIndexTimer->setInterval(UPDATE_INTERVAL_MS);

    connect(m_updateIndexTimer, &QTimer::timeout,
            [this]() { updateNow(); });
}

void IndexUpdater::updateNow()
{
    m_updateIndexTimer->stop();

    QModelIndex index;
    if (m_editorWidget != nullptr)
        index = getTargetIndexFromRootIndex(currentRootIndex());

    emit currentIndexUpdated(index);
}

int IndexUpdater::getCurrentLine() const
{
    int line = 0;
    int column = 0;
    m_editorWidget->convertPosition(m_editorWidget->position(), &line, &column);
    return line;
}

QModelIndex IndexUpdater::getTargetIndexFromRootIndex(const QModelIndex &rootIndex) const
{
    const int line = getCurrentLine();

    for (int row = 0; row < m_model->rowCount(rootIndex); ++row) {
        const QModelIndex moduleIndex = m_model->index(row, 0, rootIndex);
        const auto moduleNode = m_model->dataNode(moduleIndex);
        const Data::SourceLocation location = moduleNode->location();
        if (location.path() != currentFileName().toString())
            continue;

        const QModelIndex index = getTargetIndexFromModuleIndex(moduleIndex, line);
        if (index.isValid())
            return index;
    }

    return QModelIndex();
}

QModelIndex IndexUpdater::getTargetIndexFromModuleIndex(const QModelIndex &moduleIndex, int line) const
{
    for (int row = 0; row < m_model->rowCount(moduleIndex); ++row) {
        const QModelIndex index = m_model->index(row, 0, moduleIndex);
        const auto node = m_model->dataNode(index);
        const Data::SourceLocation location = node->location();
        if (location.line() == line)
            return index;
    }

    return QModelIndex();
}

Utils::FileName IndexUpdater::currentFileName() const
{
    return m_editorWidget->textDocument()->filePath();
}

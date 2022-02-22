/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include "synchronizedindexupdater.h"

#include <texteditor/textdocument.h>

#include <projectexplorer/project.h>
#include <projectexplorer/session.h>

#include <data/project.h>
#include <parseddatastorage.h>
#include <tree-views/model.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;

static const int UPDATE_INTERVAL_MS = 500;

SynchronizedIndexUpdater::SynchronizedIndexUpdater(const Model *model,
                                                   const ParsedDataStorage *const storage,
                                                   QObject *parent)
    : IndexUpdater(model, parent)
    , m_storage(storage)
    , m_editorWidget(nullptr)
{
    createUpdateTimer();
}

void SynchronizedIndexUpdater::setEditor(TextEditor::TextEditorWidget *editorWidget)
{
    unsetEditor();

    m_editorWidget = editorWidget;

    if (!editorEmpty()) {
        connect(m_editorWidget,
                &QPlainTextEdit::cursorPositionChanged,
                m_updateIndexTimer,
                static_cast<void (QTimer::*)()>(&QTimer::start));
        m_updateIndexTimer->start();
    } else {
        emit indexSelectionUpdated(QModelIndexList(), QModelIndex());
    }
}

void SynchronizedIndexUpdater::unsetEditor()
{
    if (!editorEmpty())
        m_editorWidget->disconnect(m_updateIndexTimer);

    m_editorWidget = nullptr;
}

void SynchronizedIndexUpdater::updateCurrentIndex()
{
    if (updateAllowed())
        updateNow();
}

namespace {

using Match = std::pair<QModelIndex, int>;

bool locationMatches(const Data::SourceLocation &cursorLocation,
                     const Data::SourceLocation &itemLocation)
{
    return itemLocation.line() == cursorLocation.line()
           && cursorLocation.column() >= itemLocation.column();
}

bool isBetterMatch(const Match &currentMatch, const Data::SourceLocation &itemLocation)
{
    return !currentMatch.first.isValid() || itemLocation.column() > currentMatch.second;
}

void findMatchInParent(const Model *model,
                       const QModelIndex &parentIndex,
                       Data::SourceLocation cursorLocation,
                       Match &currentBestMatch)
{
    for (int row = 0; row < model->rowCount(parentIndex); ++row) {
        const auto index = model->index(row, 0, parentIndex);
        const auto indexLocation = model->dataNode(index)->location();

        if (indexLocation.path() != cursorLocation.path()
            || indexLocation.line() > cursorLocation.line())
            continue;

        if (locationMatches(cursorLocation, indexLocation)
            && isBetterMatch(currentBestMatch, indexLocation))
            currentBestMatch = std::make_pair(index, indexLocation.column());

        findMatchInParent(model, index, cursorLocation, currentBestMatch);
    }
}

} // namespace

QModelIndex SynchronizedIndexUpdater::findIndexInLocation(const QModelIndex &parentIndex,
                                                          Data::SourceLocation cursorLocation) const
{
    Match found = std::make_pair(QModelIndex(), -1);

    findMatchInParent(m_model, parentIndex, cursorLocation, found);

    return found.first;
}

Data::SourceLocation SynchronizedIndexUpdater::getCurrentLocation() const
{
    int line = 0;
    int column = 0;
    m_editorWidget->convertPosition(m_editorWidget->position(), &line, &column);

    return Data::SourceLocation(currentFilePath().toString(), line, column);
}

Utils::FilePath SynchronizedIndexUpdater::currentFilePath() const
{
    return m_editorWidget->textDocument()->filePath();
}

bool SynchronizedIndexUpdater::editorEmpty() const
{
    return m_editorWidget == nullptr;
}

bool SynchronizedIndexUpdater::updateAllowed()
{
    if (editorEmpty())
        return false;

    const auto file = m_storage->getAnyFileForPath(currentFilePath());

    return file != nullptr && !file->isPolluted();
}

void SynchronizedIndexUpdater::updateNow()
{
    m_updateIndexTimer->stop();

    QModelIndex current;
    QModelIndexList selected;

    fillSelectedIndexes(current, selected);

    emit indexSelectionUpdated(selected, current);
}

void SynchronizedIndexUpdater::createUpdateTimer()
{
    m_updateIndexTimer = new QTimer(this);
    m_updateIndexTimer->setObjectName("SynchronizedIndexUpdater::UpdateTimer");
    m_updateIndexTimer->setSingleShot(true);
    m_updateIndexTimer->setInterval(UPDATE_INTERVAL_MS);

    connect(m_updateIndexTimer,
            &QTimer::timeout,
            this,
            &SynchronizedIndexUpdater::updateCurrentIndex);
}

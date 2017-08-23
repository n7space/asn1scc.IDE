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

#include "overviewindexupdater.h"

#include <texteditor/texteditor.h>

#include <coreplugin/editormanager/editormanager.h>

#include <texteditor/textdocument.h>

using namespace Asn1Acn::Internal;

static const int UPDATE_INTERVAL_MS = 500;

OverviewIndexUpdater::OverviewIndexUpdater(const OverviewModel *model)
    : m_model(model), m_editorWidget(nullptr)
{
    createUpdateTimer();
}

void OverviewIndexUpdater::setEditor(TextEditor::TextEditorWidget *editorWidget)
{
    if (m_editorWidget != nullptr)
        disconnect(m_editorWidget, &QPlainTextEdit::cursorPositionChanged,
                   this, &OverviewIndexUpdater::onCursorPositionChanged);

    m_editorWidget = editorWidget;

    if (m_editorWidget != nullptr) {
        connect(m_editorWidget, &QPlainTextEdit::cursorPositionChanged,
                this, &OverviewIndexUpdater::onCursorPositionChanged);
        m_updateIndexTimer->start();
    } else {
        emit currentIndexUpdated(QModelIndex());
    }
}

void OverviewIndexUpdater::updateCurrentIndex()
{
    if (m_editorWidget == nullptr)
        return;

    updateNow();
}

void OverviewIndexUpdater::onCursorPositionChanged()
{
    m_updateIndexTimer->start();
}

void OverviewIndexUpdater::createUpdateTimer()
{
    m_updateIndexTimer = new QTimer(this);
    m_updateIndexTimer->setObjectName("OverviewIndexUpdater::UpdateTimer");
    m_updateIndexTimer->setSingleShot(true);
    m_updateIndexTimer->setInterval(UPDATE_INTERVAL_MS);

    connect(m_updateIndexTimer, &QTimer::timeout,
            this, &OverviewIndexUpdater::onUpdateTimerTimeout);
}

void OverviewIndexUpdater::onUpdateTimerTimeout()
{
    updateNow();
}

void OverviewIndexUpdater::updateNow()
{
    m_updateIndexTimer->stop();

    QModelIndex index = getTargetIndexFromFileIndex(getCurrentFileIndex());
    emit currentIndexUpdated(index);
}

QModelIndex OverviewIndexUpdater::getCurrentFileIndex() const
{
    return QModelIndex();
}

QModelIndex OverviewIndexUpdater::getTargetIndexFromFileIndex(const QModelIndex &fileIndex) const
{
    int line = 0;
    int column = 0;
    m_editorWidget->convertPosition(m_editorWidget->position(), &line, &column);

    for (int row = 0; row < m_model->rowCount(fileIndex); ++row) {
        const QModelIndex moduleIndex = m_model->index(row, 0, fileIndex);
        const QModelIndex index = getTargetIndexFromModuleIndex(moduleIndex, line);
        if (index.isValid())
            return index;
    }

    return fileIndex;
}

QModelIndex OverviewIndexUpdater::getTargetIndexFromModuleIndex(const QModelIndex &moduleIndex, int line) const
{
    for (int row = 0; row < m_model->rowCount(moduleIndex); ++row) {
        const QModelIndex index = m_model->index(row, 0, moduleIndex);

        const ModelTreeNode *symbol = static_cast<ModelTreeNode *>(index.internalPointer());
        const Data::SourceLocation location = symbol->getSourceLocation();
        if (location.line() == line)
            return index;
    }

    return QModelIndex();
}

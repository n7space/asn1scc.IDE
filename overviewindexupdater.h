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

#pragma once

#include <QObject>
#include <QTimer>
#include <QAbstractItemModel>

#include "coreplugin/editormanager/ieditor.h"

#include "editor.h"
#include "overviewmodel.h"

namespace Asn1Acn {
namespace Internal {

class OverviewIndexUpdater : public QObject
{
    Q_OBJECT

public:
    OverviewIndexUpdater(const OverviewModel *model);
    virtual ~OverviewIndexUpdater() = default;

    void setEditor(TextEditor::TextEditorWidget *editorWidget);
    void updateCurrentIndex();

public slots:
    void onCursorPositionChanged();

signals:
    void currentIndexUpdated(const QModelIndex &modelIndex);

protected:
    QModelIndex indexForPosition(const QModelIndex &rootIndex) const;

    const OverviewModel *m_model;
    TextEditor::TextEditorWidget *m_editorWidget;

private:
    void updateNow();

    void createUpdateTimer();
    void onUpdateTimerTimeout();

    virtual QModelIndex getCurrentFileIndex() const;

    QModelIndex getTargetIndexFromFileIndex(const QModelIndex &fileIndex) const;
    QModelIndex getTargetIndexFromModuleIndex(const QModelIndex &moduleIndex, int line) const;

    QTimer *m_updateIndexTimer;
};

} /* namespace Asn1Acn */
} /* namespace Internal */

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
#pragma once

#include <memory>

#include <QModelIndex>

#include <texteditor/ioutlinewidget.h>
#include <utils/navigationtreeview.h>

#include "indexupdater.h"
#include "indexupdaterfactory.h"
#include "model.h"

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {

class TreeView : public Utils::NavigationTreeView
{
    Q_OBJECT
public:
    TreeView(QWidget *parent);

    void contextMenuEvent(QContextMenuEvent *event) override;
};

class TreeViewWidget : public TextEditor::IOutlineWidget
{
    Q_OBJECT
public:
    TreeViewWidget(Model *model, std::unique_ptr<IndexUpdaterFactory> indexUpdaterFactory);

    QList<QAction *> filterMenuActions() const override;
    void setCursorSynchronization(bool syncWithCursor) override;

public slots:
    void updateSelection(const QModelIndexList &selected, const QModelIndex &current);

protected:
    TreeView *m_treeView;

private:
    QLayout *createLayout();
    void expandItemsAbove(const QModelIndex &parent);

    std::unique_ptr<IndexUpdaterFactory> m_indexUpdaterFactory;
    std::unique_ptr<IndexUpdater> m_indexUpdater;

    Model *m_model;
};

} // namespace TreeViews
} // namespace Internal
} // namespace Asn1Acn

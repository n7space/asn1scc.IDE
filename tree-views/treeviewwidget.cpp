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
#include "treeviewwidget.h"

#include <QMenu>
#include <QVBoxLayout>

#include <coreplugin/find/itemviewfind.h>

#include "activatehandler.h"

using namespace Asn1Acn::Internal::TreeViews;

TreeView::TreeView(QWidget *parent)
    : Utils::NavigationTreeView(parent)
{
}

void TreeView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!event)
        return;

    QMenu contextMenu;

    QAction *action = contextMenu.addAction(tr("Expand All"));
    connect(action, &QAction::triggered, this, &QTreeView::expandAll);
    action = contextMenu.addAction(tr("Collapse All"));
    connect(action, &QAction::triggered, this, &QTreeView::collapseAll);

    contextMenu.exec(event->globalPos());

    event->accept();
}

TreeViewWidget::TreeViewWidget(Model::Model *model)
    : m_treeView(new TreeView(this))
    , m_model(model)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(Core::ItemViewFind::createSearchableWrapper(m_treeView));
    setLayout(layout);

    m_treeView->setModel(m_model);
    m_treeView->setExpandsOnDoubleClick(false);

    //connect(m_treeView, &QAbstractItemView::activated,
    //        this, &OverviewWidget::onItemActivated);
}

QList<QAction *> TreeViewWidget::filterMenuActions() const
{
    return {};
}

void TreeViewWidget::setCursorSynchronization(bool syncWithCursor)
{
    Q_UNUSED(syncWithCursor);
}

void TreeViewWidget::onItemActivated(const QModelIndex &index)
{
    ActivateHandler::gotoSymbol(index);
}

/*
void OverviewWidget::updateSelectionInTree(const QModelIndex &index)
{
    m_currentIndex = index;

    m_treeView->setCurrentIndex(index);
    m_treeView->scrollTo(index);
}
*/

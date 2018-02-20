/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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
#include "treeviewwidget.h"

#include <QMenu>
#include <QVBoxLayout>

#include <coreplugin/find/itemviewfind.h>

#include "expansionstaterestorer.h"
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

TreeViewWidget::TreeViewWidget(Model *model, std::unique_ptr<IndexUpdaterFactory> indexUpdaterFactory)
    : m_indexUpdaterFactory(std::move(indexUpdaterFactory))
    , m_model(model)
    , m_treeView(new TreeView(this))
{   
    model->setParent(this);

    setLayout(createLayout());

    m_treeView->setModel(model);
    m_treeView->setExpandsOnDoubleClick(false);

    connect(m_treeView, &QAbstractItemView::activated,
            [this](const QModelIndex &index){ ActivateHandler::gotoSymbol(index); });

    new ExpansionStateRestorer(m_treeView, model, this);
}

QList<QAction *> TreeViewWidget::filterMenuActions() const
{
    return {};
}

void TreeViewWidget::setCursorSynchronization(bool syncWithCursor)
{
    if (syncWithCursor)
        m_indexUpdater.reset(m_indexUpdaterFactory->createSynchronizedIndexUpdater(m_model, this));
    else
        m_indexUpdater.reset(m_indexUpdaterFactory->createUnsynchronozedIndexUpdater(m_treeView, m_model, this));

    connect(m_indexUpdater.get(), &IndexUpdater::indexSelectionUpdated,
            this, &TreeViewWidget::updateSelection);
}

QLayout *TreeViewWidget::createLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(Core::ItemViewFind::createSearchableWrapper(m_treeView));

    return layout;
}

void TreeViewWidget::updateSelection(const QModelIndexList &selected, const QModelIndex &current)
{
    auto selectionModel = m_treeView->selectionModel();

    selectionModel->select(QModelIndex(), QItemSelectionModel::Clear);

    for (const auto &item : selected) {
        expandItemsAbove(item);
        selectionModel->select(item, QItemSelectionModel::Select);
    }

    selectionModel->setCurrentIndex(current, QItemSelectionModel::NoUpdate);
    m_treeView->scrollTo(current);
}

void TreeViewWidget::expandItemsAbove(const QModelIndex &index)
{
    for (auto parent = index; parent.isValid(); parent = parent.parent())
        m_treeView->expand(parent);
}

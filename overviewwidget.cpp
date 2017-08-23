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

#include "overviewwidget.h"

#include <QVBoxLayout>
#include <QTreeView>
#include <QMenu>
#include <QEvent>
#include <QAction>
#include <QList>

#include <coreplugin/find/itemviewfind.h>
#include <coreplugin/editormanager/editormanager.h>

#include "data/sourcelocation.h"

using namespace Asn1Acn::Internal;

OverviewTreeView::OverviewTreeView(QWidget *parent) :
    Utils::NavigationTreeView(parent)
{
}

void OverviewTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

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

OverviewWidget::OverviewWidget(OverviewModel *model) :
    m_treeView(new OverviewTreeView(this)),
    m_model(model)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(Core::ItemViewFind::createSearchableWrapper(m_treeView));
    setLayout(layout);

    m_treeView->setModel(m_model);
    m_treeView->setExpandsOnDoubleClick(false);

    connect(m_treeView, &QAbstractItemView::activated,
            this, &OverviewWidget::onItemActivated);
}

OverviewWidget::~OverviewWidget()
{
    delete m_indexUpdater;
}

QList<QAction *> OverviewWidget::filterMenuActions() const
{
    return QList<QAction *>();
}

void OverviewWidget::setCursorSynchronization(bool syncWithCursor)
{
    Q_UNUSED(syncWithCursor);
}

void OverviewWidget::modelUpdated()
{
    m_treeView->expandAll();
    m_indexUpdater->updateCurrentIndex();
}

void OverviewWidget::onItemActivated(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    ModelTreeNode *node = static_cast<ModelTreeNode *>(index.internalPointer());

    const auto location = node->getSourceLocation();
    Core::EditorManager::openEditorAt(location.path(),
                                      location.line(),
                                      location.column());
}

void OverviewWidget::updateSelectionInTree(const QModelIndex &index)
{
    m_treeView->setCurrentIndex(index);
    m_treeView->scrollTo(index);
}

/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

#include "datastructureswidget.h"

#include <QVBoxLayout>
#include <QTreeView>
#include <QMenu>
#include <QEvent>
#include <QAction>
#include <QList>


#include <coreplugin/find/itemviewfind.h>

using namespace Asn1Acn::Internal;

DataStructuresTreeView::DataStructuresTreeView(QWidget *parent) :
    Utils::NavigationTreeView(parent)
{
}

void DataStructuresTreeView::contextMenuEvent(QContextMenuEvent *event)
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

DataStructuresWidget::DataStructuresWidget(AsnOverviewModel *model) :
    m_treeView(new DataStructuresTreeView(this)),
    m_model(model)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(Core::ItemViewFind::createSearchableWrapper(m_treeView));
    setLayout(layout);

    m_treeView->setModel(m_model);
}

QList<QAction *> DataStructuresWidget::filterMenuActions() const
{
    return QList<QAction *>();
}

void DataStructuresWidget::setCursorSynchronization(bool syncWithCursor)
{
    Q_UNUSED(syncWithCursor);
}

void DataStructuresWidget::modelUpdated()
{
    m_treeView->expandAll();
}

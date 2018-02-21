/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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

#include "expansionstaterestorer.h"

#include <QAbstractItemModel>
#include <QTreeView>

#include "model.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;

ExpansionStateRestorer::ExpansionStateRestorer(QTreeView *view, const Model *model, QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_model(model)
{
    connect(m_model,
            &QAbstractItemModel::modelAboutToBeReset,
            this,
            &ExpansionStateRestorer::onModelAboutToBeReset);

    connect(m_model,
            &QAbstractItemModel::modelReset,
            this,
            &ExpansionStateRestorer::onModelResetFinished);
}

void ExpansionStateRestorer::tryAddIndex(const QModelIndex &index)
{
    if (!m_view->isExpanded(index))
        return;

    m_expandedItems << m_model->data(index, Qt::DisplayRole).toString();
    addChildrenIndexes(index);
}

void ExpansionStateRestorer::addChildrenIndexes(const QModelIndex &parent)
{
    for (int i = 0; i < m_model->rowCount(parent); ++i)
        for (int j = 0; j < m_model->columnCount(parent); ++j)
            tryAddIndex(m_model->index(i, j, parent));
}

void ExpansionStateRestorer::onModelAboutToBeReset()
{
    m_expandedItems.clear();
    addChildrenIndexes(QModelIndex());
}

void ExpansionStateRestorer::onModelResetFinished()
{
    for (const auto &item : m_expandedItems) {
        const auto matches = m_model->match(m_model->index(0, 0, QModelIndex()),
                                            Qt::DisplayRole,
                                            item,
                                            -1,
                                            Qt::MatchRecursive);

        for (const auto &match : matches)
            m_view->expand(match);
    }
}

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

#include "overviewmodel.h"

namespace Asn1Acn {
namespace Internal {

OverviewModel::OverviewModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_rootItem(nullptr)
{
}

OverviewModel::~OverviewModel()
{
}

int OverviewModel::columnCount(const QModelIndex &parent) const
{
    const ParsedTreeNode *symbol = getValidNode(parent);

    return symbol != nullptr ? symbol->columnCount() : 0;
}

int OverviewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const ParsedTreeNode *symbol = getValidNode(parent);

    return symbol != nullptr ? symbol->childrenCount() : 0;
}

QVariant OverviewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    ParsedTreeNode *item = static_cast<ParsedTreeNode *>(index.internalPointer());

    return item->data();
}

Qt::ItemFlags OverviewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QVariant OverviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    return QVariant();
}

QModelIndex OverviewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const ParsedTreeNode *parentItem = getValidNode(parent);
    if (parentItem == nullptr)
        return QModelIndex();

    ParsedTreeNode::ParsedTreeNodePtr childItem = parentItem->childAt(row);
    if (childItem == nullptr)
        return QModelIndex();

    return createIndex(row, column, childItem.get());
}

QModelIndex OverviewModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ParsedTreeNode *child = static_cast<ParsedTreeNode *>(index.internalPointer());
    const ParsedTreeNode *parent = child->parent();

    if (parent == nullptr || parent == m_rootItem.get())
        return QModelIndex();

    return createIndex(parent->row(), 0, const_cast<ParsedTreeNode *>(parent));
}

void OverviewModel::setRootNode(ParsedTreeNode::ParsedTreeNodePtr root)
{
    beginResetModel();

    m_rootItem = root;

    endResetModel();
}

const ParsedTreeNode *OverviewModel::getValidNode(const QModelIndex &index) const
{
    ParsedTreeNode *node = nullptr;

    if (!index.isValid())
        node = m_rootItem.get();
    else
        node = static_cast<ParsedTreeNode *>(index.internalPointer());

    return node;
}

} // namespace Internal
} // namespace Asn1Acn

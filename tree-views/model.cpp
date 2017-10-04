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
#include "model.h"

#include "displayrolevisitor.h"

using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::Data;

Model::Model(QObject *parent)
    : QAbstractItemModel(parent)
    , m_root(nullptr)
{
    // TODO - connect to some global index store?
    /*
     connect(ModelTree::instance(), &ModelTree::modelAboutToUpdate,
            [this](){ beginResetModel(); });

    connect(ModelTree::instance(), &ModelTree::modelUpdated,
            [this](){ endResetModel(); });
     * */
}

Model::~Model()
{
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto node = dataNode(index);

    switch (role) {
    case Qt::DecorationRole:
        // TODO icons visitor
        return QVariant();
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return node->valueFor<DisplayRoleVisitor>();
    }

    return QVariant();
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    return QVariant();
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    if (column > 0)
        return QModelIndex();

    const auto parentNode = dataNode(parent);
    if (parentNode == nullptr) {
        if (row == 0 && column == 0)
            return createIndex(row, column, const_cast<Data::Node*>(m_root));
        return QModelIndex();
    }

    const auto node = nthChild(parentNode, row);
    if (node == nullptr)
        return QModelIndex();

    return createIndex(row, column, node);
}

QModelIndex Model::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    const auto node = dataNode(index);
    const auto parent = parentOf(node);

    if (parent == nullptr)
        return QModelIndex();

    return createIndex(indexInParent(parent, node), 0, parent);
}

int Model::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    auto node = dataNode(parent);
    return childrenCount(node);
}

int Model::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void Model::setRoot(const Node *root)
{
    beginResetModel();
    m_root = root;
    endResetModel();
}

const Node *Model::dataNode(const QModelIndex &index)
{
    return static_cast<Node*>(index.internalPointer());
}

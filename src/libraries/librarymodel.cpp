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

#include "librarymodel.h"

using namespace Asn1Acn::Internal::Libraries;

LibraryModel::LibraryModel(const Metadata::LibraryNode *root, QObject *parent)
    : QAbstractItemModel(parent)
    , m_root(root)
{
}

QVariant LibraryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto node = dataNode(index);

    switch (role) {
    case Qt::DisplayRole:
        return node->name();
    case Qt::ToolTipRole:
        return node->description();
    case Qt::CheckStateRole:
        return node->checked() ? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

QModelIndex LibraryModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    auto parentNode = dataNode(parent);

    const auto node = parentNode->child(row);
    if (node == nullptr)
        return QModelIndex();

    return createIndex(row, column, node);
}

QModelIndex LibraryModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    const auto node = dataNode(index);
    const auto parent = node->parent();

    if (parent == m_root || parent == nullptr)
        return QModelIndex();

    return createIndex(parent->parent()->childIndex(parent), index.column(), parent);
}

int LibraryModel::rowCount(const QModelIndex &parent) const
{
    return dataNode(parent)->childrenCount();
}

int LibraryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

const Metadata::LibraryNode *LibraryModel::dataNode(const QModelIndex &index) const
{
    return index.isValid() ? static_cast<Metadata::LibraryNode *>(index.internalPointer()) : m_root;
}

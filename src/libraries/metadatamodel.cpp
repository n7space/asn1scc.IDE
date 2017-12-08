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

#include "metadatamodel.h"

#include <utils/qtcassert.h>

#include "metadatacheckstatehandler.h"

#include <QDebug>

using namespace Asn1Acn::Internal::Libraries;

MetadataModel::MetadataModel(const Metadata::LibraryNode *root, QObject *parent)
    : QAbstractItemModel(parent)
    , m_root(root)
{
}

QVariant MetadataModel::data(const QModelIndex &index, int role) const
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
        return node->checked();
    }

    return QVariant();
}

Qt::ItemFlags MetadataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

QModelIndex MetadataModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const auto parentNode = dataNode(parent);
    const auto node = parentNode->child(row);

    if (node == nullptr)
        return QModelIndex();

    return createIndex(row, column, node);
}

QModelIndex MetadataModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    const auto node = dataNode(index);
    const auto parent = node->parent();

    if (parent == m_root || parent == nullptr)
        return QModelIndex();

    return createIndex(parent->parent()->childIndex(parent), index.column(), parent);
}

int MetadataModel::rowCount(const QModelIndex &parent) const
{
    return dataNode(parent)->childrenCount();
}

int MetadataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

bool MetadataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        if (!index.isValid())
            return false;

        MetadataCheckStateHandler checkHandler(this);

        if (checkHandler.changeCheckStates(index, static_cast<Qt::CheckState>(value.toInt())))
            selectItems(checkHandler.changedIndexes());
        else
            itemConflicted(checkHandler.conflict());

        return true;
    }

    return QAbstractItemModel::setData(index, value, role);
}

const Metadata::LibraryNode *MetadataModel::dataNode(const QModelIndex &index) const
{
    return index.isValid() ? static_cast<Metadata::LibraryNode *>(index.internalPointer()) : m_root;
}

QModelIndex MetadataModel::rootIndex() const
{
    return createIndex(0, 0, const_cast<Metadata::LibraryNode *>(m_root));
}

void MetadataModel::selectItems(MetadataCheckStateHandler::States &items)
{
    for (auto it = items.begin(); it != items.end(); ++it) {
        static_cast<Metadata::LibraryNode *>(it.key().internalPointer())->setChecked(it.value());
        emit dataChanged(it.key(), it.key());
    }
}

void MetadataModel::itemConflicted(MetadataCheckStateHandler::Conflict &conflict) const
{
    QTC_ASSERT(!conflict.first.isEmpty(), return);
    QTC_ASSERT(!conflict.second.isEmpty(), return);

    emit conflictOccurred(conflict.first, conflict.second);
}

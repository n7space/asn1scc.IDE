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

#include <QAbstractItemModel>
#include <QModelIndex>

#include <libraries/metadata/librarynode.h>

#include "metadatacheckstatehandler.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {

class MetadataModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MetadataModel(const Metadata::LibraryNode *root, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    const Metadata::LibraryNode *dataNode(const QModelIndex &index) const;
    const QHash<QPersistentModelIndex, Qt::CheckState> &selectedItems() const;

signals:
    void conflictOccurred(const QString &first, const QString &second) const;

private:
    void selectItems(const MetadataCheckStateHandler::States &items);
    void itemConflicted(const MetadataCheckStateHandler::Conflict &conflict) const;

    const Metadata::LibraryNode *m_root;
    QHash<QPersistentModelIndex, Qt::CheckState> m_selectedItems;
};

} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

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

#include "asnoverviewmodel.h"

namespace Asn1Acn {
namespace Internal {

AsnOverviewModel::AsnOverviewModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_rootItem(new QVariant()),
    m_asnParsedDocument(nullptr)
{
}

AsnOverviewModel::~AsnOverviewModel()
{
    delete m_rootItem;
}

int AsnOverviewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant AsnOverviewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    QVariant *item = static_cast<QVariant *>(index.internalPointer());

    return *item;
}

Qt::ItemFlags AsnOverviewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QVariant AsnOverviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    return *m_rootItem;
}

QModelIndex AsnOverviewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid() && m_asnParsedDocument) {
        QVariant *childItem = m_asnParsedDocument->at(row);
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex AsnOverviewModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int AsnOverviewModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid() && m_asnParsedDocument)
        return m_asnParsedDocument->getCount();

    return 0;
}

void AsnOverviewModel::setDocument(std::shared_ptr<AsnParsedDocument> file)
{
    beginResetModel();
    m_asnParsedDocument = file;
    endResetModel();
}

} // namespace Internal
} // namespace Asn1Acn

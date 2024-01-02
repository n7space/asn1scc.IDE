/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include <data/node.h>

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {

class Model : public QAbstractItemModel
{
    Q_OBJECT
protected:
    explicit Model(QObject *parent = 0);

public:
    virtual ~Model();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    void setRoot(const Data::Node *root);

    const Data::Node *dataNode(const QModelIndex &index) const;

protected slots:
    virtual void onEndResetModel() = 0;

protected:
    virtual Data::Node *parentOf(const Data::Node *node) const = 0;
    virtual int childrenCount(const Data::Node *node) const = 0;
    virtual int indexInParent(const Data::Node *parent, const Data::Node *node) const = 0;
    virtual Data::Node *nthChild(const Data::Node *node, int n) const = 0;

private:
    const Data::Node *m_root;
};

} // namespace TreeViews
} // namespace Internal
} // namespace Asn1Acn

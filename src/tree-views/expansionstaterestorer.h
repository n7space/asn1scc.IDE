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
#pragma once

#include <QObject>

class QTreeView;

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {

class Model;

class ExpansionStateRestorer : public QObject
{
    Q_OBJECT
public:
    ExpansionStateRestorer(QTreeView *view, const Model *model, QObject *parent = nullptr);

private slots:
    void onModelAboutToBeReset();
    void onModelResetFinished();

private:
    void tryAddIndex(const QModelIndex &index);
    void addChildrenIndexes(const QModelIndex &parent);

    QStringList m_expandedItems;

    QTreeView *m_view;
    const Model *const m_model;
};

} /* namespace TreeViews */
} /* namespace Asn1Acn */
} /* namespace Internal */

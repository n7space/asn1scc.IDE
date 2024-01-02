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

#include <QHash>
#include <QModelIndex>

#include <libraries/metadata/reference.h>

namespace Asn1Acn {
namespace Internal {
namespace Libraries {

class MetadataModel;

class MetadataCheckStateHandler
{
public:
    using States = QHash<QModelIndex, Qt::CheckState>;
    using Conflict = QPair<QString, QString>;

    MetadataCheckStateHandler(const MetadataModel *model);

    bool changeCheckStates(const QModelIndex &index, const Qt::CheckState state);

    const States &changedIndexes() const;
    const Conflict &conflict() const;

private:
    bool uncheck(const QModelIndex &index);
    bool check(const QModelIndex &index, const Qt::CheckState state);

    bool updateChildren(const QModelIndex &parent, const Qt::CheckState state);
    bool updateParent(const QModelIndex &index, const Qt::CheckState childState);

    bool handleRelatives(const QModelIndex &index);

    bool hasNoConflicts(const QModelIndex &index);
    void enqueueRequired(const QModelIndex &index);

    QModelIndex findRelative(const Metadata::Reference &reference) const;
    QModelIndex findChildByName(const QModelIndex &parent, const QString &name) const;

    Qt::CheckState parentState(const QModelIndex &index, const Qt::CheckState state) const;

    States m_changedIndexes;
    Conflict m_conflict;

    QList<QModelIndex> m_indexesToCheck;

    const MetadataModel *m_model;
};

} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

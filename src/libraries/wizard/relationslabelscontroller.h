/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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

#include <QModelIndex>
#include <QTreeWidget>

#include <libraries/metadatamodel.h>

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Wizard {

class RelationsLabelsController : public QObject
{
    Q_OBJECT

public:
    RelationsLabelsController(MetadataModel *model,
                              QTreeWidget *requiresTree,
                              QTreeWidget *conflictsTree,
                              QObject *parent = nullptr);

public slots:
    void onFocusedItemChanged(const QModelIndex &current, const QModelIndex &previous) const;

private:
    void fillRelations(const QModelIndex &index, QList<Metadata::Reference> &requirements, QList<Metadata::Reference> &conflicts) const;

    MetadataModel *m_model;

    QTreeWidget *m_requiresTree;
    QTreeWidget *m_conflictsTree;
};

} // namespace Wizard
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

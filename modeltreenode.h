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

#pragma once

#include <memory>

#include <QString>
#include <QVariant>
#include <QList>

#include "data/sourcelocation.h"

namespace Asn1Acn {
namespace Internal {

class ModelTreeNode
{
public:

    using ModelTreeNodePtr = std::shared_ptr<ModelTreeNode>;

    ModelTreeNode(const QString &name = QString::null, const Data::SourceLocation location = Data::SourceLocation());

    int childrenCount() const;

    ModelTreeNodePtr childAt(int idx) const;
    ModelTreeNodePtr getChildByName(const QString &name) const;

    void addChild(ModelTreeNodePtr child);

    void removeChildByName(const QString &name);
    void removeChildren();

    QString name() const;

    const ModelTreeNode *parent() const;
    ModelTreeNode *parent();

    int indexInParent() const;

    const Data::SourceLocation &sourceLocation() const;

private:
    QString m_name;

    ModelTreeNode *m_parent;
    QList<ModelTreeNodePtr> m_children;

    const Data::SourceLocation m_location;
};

} // namespace Internal
} // namespace Asn1Acn

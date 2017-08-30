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
#include "data/type.h"

namespace Asn1Acn {
namespace Internal {

class ModelTreeNode
{ 
protected:
    ModelTreeNode(const QString &name, Data::Type type, const Data::SourceLocation location);

public:
    using ModelTreeNodePtr = std::shared_ptr<ModelTreeNode>;

    static ModelTreeNodePtr makePtr(const QString &name = QString::null,
                                    Data::Type type = Data::Type::UserDefined,
                                    const Data::SourceLocation location = Data::SourceLocation());

    int childrenCount() const;

    ModelTreeNodePtr childAt(int idx) const;
    ModelTreeNodePtr getChildByName(const QString &name) const;

    void addChild(ModelTreeNodePtr child);

    void removeChildByName(const QString &name);
    void removeChildren();

    const QString& name() const { return m_name; }
    Data::Type type() const { return m_type; }

    const ModelTreeNode *parent() const;
    ModelTreeNode *parent();

    int indexInParent() const;

    const Data::SourceLocation &sourceLocation() const;

private:
    QString m_name;
    Data::Type m_type;

    ModelTreeNode *m_parent;
    QList<ModelTreeNodePtr> m_children;

    const Data::SourceLocation m_location;
};

} // namespace Internal
} // namespace Asn1Acn

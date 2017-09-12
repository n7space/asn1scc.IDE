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

#include <QString>

#include "sourcelocation.h"
#include "type.h"
#include "node.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

// TODO not a node?
class TypeReference : public Node
{
public:
    TypeReference(const Type dataType, const SourceLocation &location);
    TypeReference(const QString &name, const QString &module, const SourceLocation &location);
    // TODO kill
    TypeReference() : TypeReference(Type::UserDefined, {}) {}
    ~TypeReference() override;

    QVariant accept(const Visitor &visitor) const override;

    int childrenCount() const override;
    int childIndex(const NodeConstPtr &child) const override;

    const QString &name() const { return m_name; }
    const QString &module() const { return m_module; }
    Type type() const { return m_type; }
    bool isUserDefined() const { return m_type == Type::UserDefined; }

private:
    Type m_type;
    QString m_name;
    QString m_module;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

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
#include "displayrolevisitor.h"

#include <data/definitions.h>
#include <data/file.h>
#include <data/typeassignment.h>
#include <data/typereference.h>
#include <data/project.h>

using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::Data;

DisplayRoleVisitor::~DisplayRoleVisitor()
{
}

QString DisplayRoleVisitor::valueFor(const Definitions &defs) const
{
    return defs.name();
}

QString DisplayRoleVisitor::valueFor(const File &file) const
{
    return file.location().path();
}

QString DisplayRoleVisitor::valueFor(const TypeAssignment &type) const
{
    switch (type.type().m_kind) {
    case Type::Kind::Boolean:
        return type.name() + ": " + "BOOLEAN";
    case Type::Kind::Null:
        return type.name() + ": " + "NULL";
    case Type::Kind::Integer:
        return type.name() + ": " + "INTEGER";
    case Type::Kind::Real:
        return type.name() + ": " + "REAL";
    case Type::Kind::BitString:
        return type.name() + ": " + "BIT STRING";
    case Type::Kind::OctetString:
        return type.name() + ": " + "OCTET STRING";
    case Type::Kind::IA5String:
        return type.name() + ": " + "IA5String";
    case Type::Kind::NumericString:
        return type.name() + ": " + "NumericString";
    case Type::Kind::Enumerated:
        return type.name() + ": " + "ENUMERATED";
    case Type::Kind::Choice:
        return type.name() + ": " + "CHOICE";
    case Type::Kind::Sequence:
        return type.name() + ": " + "SEQUENCE";
    case Type::Kind::SequenceOf:
        return type.name() + ": " + "SEQUENCE OF";
    case Type::Kind::UserDefined:
        return type.name() + ": " + type.type().m_module + "." + type.type().m_name;
    }

    return {};
}

QString DisplayRoleVisitor::valueFor(const TypeReference &ref) const
{
    Q_UNUSED(ref);
    return {};
}

QString DisplayRoleVisitor::valueFor(const Project &project) const
{
    return project.name();
}

QString DisplayRoleVisitor::valueFor(const Root &root) const
{
    Q_UNUSED(root);
    return {};
}

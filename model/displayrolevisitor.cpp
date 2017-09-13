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

#include "../data/definitions.h"
#include "../data/file.h"
#include "../data/typeassignment.h"
#include "../data/typereference.h"

using namespace Asn1Acn::Internal::Model;
using namespace Asn1Acn::Internal::Data;

DisplayRoleVisitor::~DisplayRoleVisitor()
{
}

QVariant DisplayRoleVisitor::visit(const Definitions &defs) const
{
    return defs.name();
}

QVariant DisplayRoleVisitor::visit(const File &file) const
{
    return file.name();
}

QVariant DisplayRoleVisitor::visit(const TypeAssignment &type) const
{
    return QString(type.name() + ": " + type.reference().accept(*this).toString());
}

QVariant DisplayRoleVisitor::visit(const TypeReference &ref) const
{
    switch (ref.type()) {
    case Type::Boolean:
        return "BOOLEAN";
    case Type::Null:
        return "NULL";
    case Type::Integer:
        return "INTEGER";
    case Type::Real:
        return "REAL";
    case Type::BitString:
        return "BIT STRING";
    case Type::OctetString:
        return "OCTET STRING";
    case Type::IA5String:
        return "IA5String";
    case Type::NumericString:
        return "NumericString";
    case Type::Enumerated:
        return "ENUMERATED";
    case Type::Choice:
        return "CHOICE";
    case Type::Sequence:
        return "SEQUENCE";
    case Type::SequenceOf:
        return "SEQUENCE OF";
    case Type::UserDefined:
        return QString(ref.module() + "." + ref.name());
    }
    return QVariant();
}

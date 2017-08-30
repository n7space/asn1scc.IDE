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

namespace Asn1Acn {
namespace Internal {
namespace Data {

class TypeReference
{
public:
    enum class DataType {
        Boolean,
        Null,
        Integer,
        Real,
        BitString,
        OctetString,
        IA5String,
        NumericString,
        Enumerated,
        Choice,
        Sequence,
        SequenceOf,

        UserDefined
    };

    TypeReference(const DataType dataType, const SourceLocation &location)
        : m_type(dataType), m_location(location)
    {}

    TypeReference(const QString &name, const QString &module, const SourceLocation &location)
        : m_type(DataType::UserDefined), m_name(name), m_location(location), m_module(module)
    {}

    TypeReference() = default;

    const QString &name() const { return m_name; }
    const QString &module() const { return m_module; }
    const SourceLocation &location() const { return m_location; }
    const DataType &type() const { return m_type; }

private:
    DataType m_type;
    QString m_name;
    SourceLocation m_location;
    QString m_module;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

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

#include <map>

#include <QString>

#include "typeassignment.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class Definitions
{
public:
    Definitions(const QString &name, const SourceLocation &location)
        : m_name(name), m_location(location)
    {}

    const QString &name() const { return m_name; }
    const SourceLocation &location() const { return m_location; }

    void add(const TypeAssignment &type)
    {
        m_types.insert(std::make_pair(type.name(), type));
    }

    void addImportedType(const QString &typeName)
    {
        m_importedTypes.append(typeName);
    }

    using Types = std::map<QString, TypeAssignment>;

    const Types &types() const { return m_types; }
    const QList<QString> &importedTypes() { return m_importedTypes; }

private:
    QString m_name;
    SourceLocation m_location;
    Types m_types;
    QList<QString> m_importedTypes;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

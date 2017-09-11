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
#include <list>
#include <memory>

#include <QString>
#include <QStringList>

#include "typeassignment.h"
#include "node.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class Definitions : public Node
{
public:
    Definitions(const QString &name, const SourceLocation &location)
        : Node(location)
        , m_name(name)
    {}

    ~Definitions() override;

    const QString &name() const { return m_name; }

    int childrenCount() const override;

    void add(const TypeAssignmentPtr &type)
    {
        m_typeByNameMap.insert({ type->name(), type });
        m_types.push_back(type);
    }

    void addImportedType(const QString &typeName)
    {
        m_importedTypes.append(typeName);
    }

    using Types = std::list<TypeAssignmentPtr>;

    const Types &types() const { return m_types; }
    TypeAssignmentPtr type(const QString &name) const;
    const QStringList &importedTypes() { return m_importedTypes; }

private:
    QString m_name;
    Types m_types;
    std::map<QString, TypeAssignmentPtr> m_typeByNameMap;
    QStringList m_importedTypes;
};

using DefinitionsPtr = std::shared_ptr<Definitions>;

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

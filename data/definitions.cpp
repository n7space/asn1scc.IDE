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
#include "definitions.h"

#include <utils/qtcassert.h>

#include "visitor.h"

using namespace Asn1Acn::Internal::Data;

Definitions::Definitions(const QString &name, const SourceLocation &location)
    : Node(location)
    , m_name(name)
{}

Definitions::~Definitions()
{
}

QVariant Definitions::accept(const Visitor &visitor) const
{
    return visitor.visit(*this);
}

void Definitions::add(const TypeAssignmentPtr &type)
{
    m_typeByNameMap.insert({ type->name(), type });
    m_types.push_back(type);
    type->setParent(shared_from_this());
}

void Definitions::addImportedType(const QString &typeName)
{
    m_importedTypes.append(typeName);
}

TypeAssignmentPtr Definitions::type(const QString &name) const
{
    const auto it = m_typeByNameMap.find(name);
    if (it == m_typeByNameMap.end())
        return {};
    return it->second;
}

int Definitions::childrenCount() const
{
    return static_cast<int>(m_types.size());
}

int Definitions::childIndex(const NodeConstPtr &child) const
{
    const auto it = std::find(m_types.begin(), m_types.end(), child);
    QTC_ASSERT(it != m_types.end(), return -1);
    return std::distance(m_types.begin(), it);
}

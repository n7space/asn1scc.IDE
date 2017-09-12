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
#include "file.h"

#include <utils/qtcassert.h>

#include "visitor.h"

using namespace Asn1Acn::Internal::Data;

File::File(const QString &filePath)
    : Node({filePath, 0, 0})
{
}

File::~File()
{
}

QVariant File::accept(const Visitor &visitor) const
{
    return visitor.visit(*this);
}

DefinitionsPtr File::definitions(const QString &name) const
{
    const auto it = m_definitionsByNameMap.find(name);
    if (it == m_definitionsByNameMap.end())
        return {};
    return it->second;
}

int File::childrenCount() const
{
    return static_cast<int>(m_definitionsList.size());
}

void File::add(const DefinitionsPtr &defs)
{
    m_definitionsByNameMap[defs->name()] = defs;
    m_definitionsList.push_back(defs);
    defs->setParent(shared_from_this());
}

int File::childIndex(const NodeConstPtr &child) const
{
    const auto it = std::find(m_definitionsList.begin(), m_definitionsList.end(), child);
    QTC_ASSERT(it != m_definitionsList.end(), return -1);
    return std::distance(m_definitionsList.begin(), it);
}

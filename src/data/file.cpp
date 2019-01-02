/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
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

#include "visitor.h"

using namespace Asn1Acn::Internal::Data;

File::File(const QString &path)
    : Node(path, {path, 0, 0})
    , m_polluted(false)
{}

File::~File() {}

void File::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

const Definitions *File::definitions(const QString &name) const
{
    const auto it = m_definitionsByNameMap.find(name);
    return it != m_definitionsByNameMap.end() ? it->second : nullptr;
}

void File::add(std::unique_ptr<Definitions> defs)
{
    defs->setParent(this);
    m_definitionsByNameMap[defs->name()] = defs.get();
    m_definitionsList.push_back(std::move(defs));
}

void File::addTypeReference(std::unique_ptr<TypeReference> ref)
{
    m_referencesMap.insert(std::make_pair(ref->location().line(), ref.get()));
    m_references.push_back(std::move(ref));
}

void File::addErrorMessage(const ErrorMessage &message)
{
    m_errorList.push_back(message);
}

void File::clearReferences()
{
    m_referencesMap.clear();
    m_references.clear();
}

void File::clearErrors()
{
    m_errorList.clear();
}

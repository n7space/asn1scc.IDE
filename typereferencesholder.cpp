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

#include <QList>

#include "typereferencesholder.h"

using namespace Asn1Acn::Internal;

void TypeReferencesHolder::addReference(const QString &path, const Data::File &file)
{
    ReferenceLookup lookup;

    for (const auto &definitions : file.definitionsList())
        createReferencesFromModule(*definitions, lookup);

    m_references.insert(path, lookup);
}

void TypeReferencesHolder::removeReference(const QString &path)
{
    m_references.remove(path);
}

void TypeReferencesHolder::createReferencesFromModule(const Data::Definitions &moduleDefinition,
                                                      ReferenceLookup &lookup) const
{
    for (const auto &type : moduleDefinition.types()) {
        auto reference = type->reference();
        if (reference.isUserDefined())
            lookup.insert(type->location().line(), reference);
    }
}

Data::TypeReference TypeReferencesHolder::getTypeReference(const QString &path, const int line, const int col) const
{
    if (!m_references.contains(path))
        return Data::TypeReference();

    const ReferenceLookup &lookup = m_references.value(path);

    if (!lookup.contains(line))
        return Data::TypeReference();

    QList<Data::TypeReference> typeRefs = lookup.values(line);
    foreach (const Data::TypeReference &typeRef, typeRefs) {
        Data::SourceLocation sourceLocation = typeRef.location();

        if (col >= sourceLocation.column() && col <= sourceLocation.column() + typeRef.name().size())
            return typeRef;
    }

    return Data::TypeReference();
}

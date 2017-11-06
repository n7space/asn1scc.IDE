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
#include "importfindingvisitor.h"

#include <data/root.h>
#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>

using namespace Asn1Acn::Internal::Completion;
using namespace Asn1Acn::Internal::Data;

ImportFindingVisitor::ImportFindingVisitor(const QString &module, const QString &type)
    : m_module(module)
    , m_type(type)
{
}

TypeAssignment *ImportFindingVisitor::valueFor(const Root &root) const
{
    for (const auto &project : root.projects()) {
        const auto res = valueFor(*project);
        if (res != nullptr)
            return res;
    }
    return nullptr;
}

TypeAssignment *ImportFindingVisitor::valueFor(const Definitions &defs) const
{
    if (defs.name() != m_module)
        return nullptr;
    for (const auto &type : defs.types())
        if (type->name() == m_type)
            return type.get();
    return nullptr;
}

TypeAssignment *ImportFindingVisitor::valueFor(const File &file) const
{
    const auto defs = file.definitions(m_module);
    return defs == nullptr ? nullptr : valueFor(*defs);
}

TypeAssignment *ImportFindingVisitor::valueFor(const TypeAssignment &type) const
{
    Q_UNUSED(type);
    return nullptr;
}

TypeAssignment *ImportFindingVisitor::valueFor(const VariableAssignment &variable) const
{
    Q_UNUSED(variable);
    return nullptr;
}

TypeAssignment *ImportFindingVisitor::valueFor(const TypeReference &ref) const
{
    Q_UNUSED(ref);
    return nullptr;
}

TypeAssignment *ImportFindingVisitor::valueFor(const Project &project) const
{
    for (const auto &file : project.files()) {
        const auto res = valueFor(*file);
        if (res != nullptr)
            return res;
    }
    return nullptr;
}

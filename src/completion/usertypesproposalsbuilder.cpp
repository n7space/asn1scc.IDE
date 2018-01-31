/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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
#include "usertypesproposalsbuilder.h"

#include <tree-views/decorationrolevisitor.h>

#include "importfindingvisitor.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Completion;

UserTypesProposalsBuilder::UserTypesProposalsBuilder(const Data::File *data)
    : m_data(data)
{
}

void UserTypesProposalsBuilder::fillProposals()
{
    if (m_data == nullptr)
        return;

    for (const auto &definitions : m_data->definitionsList()) {
        appendImportedTypes(definitions->importedTypes());
        appendInternalTypes(definitions->types());
        appendImportedValues(definitions->importedValues());
        appendInternalValues(definitions->values());
    }
}

void UserTypesProposalsBuilder::appendInternalTypes(const Data::Definitions::Types &types)
{
    for (const auto &type : types)
        addProposal(type->name(), type->valueFor<TreeViews::DecorationRoleVisitor>());
}

void UserTypesProposalsBuilder::appendImportedTypes(const Data::Definitions::ImportedTypes &importedTypes)
{
    foreach (const auto &importedType, importedTypes)
        appendImportedElement(importedType.module(), importedType.name());
}

void UserTypesProposalsBuilder::appendInternalValues(const Data::Definitions::Values &values)
{
    for (const auto &value : values)
        addProposal(value->name(), value->valueFor<TreeViews::DecorationRoleVisitor>());
}

void UserTypesProposalsBuilder::appendImportedValues(const Data::Definitions::ImportedValues &importedValues)
{
    foreach (const auto &importedValue, importedValues)
        appendImportedElement(importedValue.module(), importedValue.name());
}

void UserTypesProposalsBuilder::appendImportedElement(const QString &module, const QString &name)
{
    static const auto defaultIcon = QIcon(":/codemodel/images/member.png");
    const auto searchRoot = m_data->parent() != nullptr ? m_data->parent() : m_data;
    const auto node = searchRoot->valueFor<ImportFindingVisitor>(module, name);
    const auto icon = node == nullptr ? defaultIcon : node->valueFor<TreeViews::DecorationRoleVisitor>();
    addProposal(name, icon);
}

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
#include "usertypesproposalsbuilder.h"

#include <data/definitions.h>

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
    }
}

void UserTypesProposalsBuilder::appendInternalTypes(const Data::Definitions::Types &types)
{
    for (const auto &type : types)
        addProposal(type->name(), type->valueFor<TreeViews::DecorationRoleVisitor>());
}

void UserTypesProposalsBuilder::appendImportedTypes(const Data::Definitions::ImportedTypes &importedTypes)
{
    const auto searchRoot = m_data->parent() != nullptr ? m_data->parent() : m_data;
    static const auto defaultIcon = QIcon(":/codemodel/images/member.png");
    foreach(const auto &importedType, importedTypes) {
        const auto typeNode = searchRoot->valueFor<ImportFindingVisitor>(importedType.module(), importedType.name());
        const auto icon = typeNode == nullptr ? defaultIcon : typeNode->valueFor<TreeViews::DecorationRoleVisitor>();
        addProposal(importedType.name(), icon);
    }
}

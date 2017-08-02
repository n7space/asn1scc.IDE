/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

#include "usertypesproposalsprovider.h"

#include "../data/definitions.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Completion;

UserTypesProposalsProvider::UserTypesProposalsProvider(const std::unique_ptr<Data::Modules> &data)
    : ProposalsProvider(":/codemodel/images/member.png"), m_data(data)
{
}

Proposals UserTypesProposalsProvider::createProposals() const
{
    Proposals proposals;

    Data::Modules::DefinitionsMap::const_iterator defIt;
    for (defIt = m_data->definitions().begin(); defIt != m_data->definitions().end(); defIt++) {

        const std::unique_ptr<Data::Definitions> &definitions = defIt->second;
        proposals.append(createImportedTypes(definitions->importedTypes()));

        const Data::Definitions::Types &types = definitions->types();
        proposals.append(createInternalTypes(types));
    }

    return proposals;
}

Proposals UserTypesProposalsProvider::createInternalTypes(const Data::Definitions::Types &types) const
{
    Proposals proposals;

    for (auto typeIt = types.begin(); typeIt != types.end(); typeIt++)
        addProposal(proposals, typeIt->second.name());

    return proposals;
}

Proposals UserTypesProposalsProvider::createImportedTypes(const QList<QString> &importedProposals) const
{
    Proposals proposals;

    foreach(const QString &typeName, importedProposals)
        addProposal(proposals, typeName);

    return proposals;
}

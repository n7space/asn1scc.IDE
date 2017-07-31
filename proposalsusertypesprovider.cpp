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

#include "proposalsusertypesprovider.h"

#include "data/definitions.h"

using namespace Asn1Acn::Internal;

PoroposalsUserTypesProvider::PoroposalsUserTypesProvider(const std::unique_ptr<Data::Modules> &data)
    : ProposalItemsProvider(":/codemodel/images/member.png"), m_data(data)
{
}

QList<TextEditor::AssistProposalItemInterface *> PoroposalsUserTypesProvider::createProposals() const
{
    QList<TextEditor::AssistProposalItemInterface *> proposals;

    Data::Modules::DefinitionsMap::const_iterator defIt;
    for (defIt = m_data->definitions().begin(); defIt != m_data->definitions().end(); defIt++) {

        const Data::Definitions::Types &types = defIt->second->types();

        Data::Definitions::Types::const_iterator typeIt;
        for (typeIt = types.begin(); typeIt != types.end(); typeIt++)
            addProposal(proposals, typeIt->second.name());
    }

    // TODO: this should provide information about imported types also, but those are not parsed yet

    return proposals;
}

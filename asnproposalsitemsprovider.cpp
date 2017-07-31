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

#include <QString>

#include "asnproposalsitemsprovider.h"

using namespace Asn1Acn::Internal;

const char *KEYWORDS[] = { "PLUS-INFINITY", "MINUS-INFINITY", "AUTOMATIC", "TAGS", "EXPLICIT", "IMPLICIT",
                           "BEGIN", "END", "ALL EXCEPT", "EXPORTS", "IMPORTS", "DEFINITIONS", "FROM",
                           "APPLICATION", "PRIVATE", "UNIVERSAL", "EXCEPT", "UNION", "INTERSECTION FROM", "" };

const char *TYPES[] = { "ENUMERATED", "INTEGER", "REAL", "BOOLEAN", "CHOICE", "SET", "SET OF", "SEQUENCE",
                        "SEQUENCE OF", "OCTET STRING", "BIT STRING", "" };

const char *BUILTIN[] = { "NULL", "FALSE", "TRUE", ""};

const char *ATTRIBUTES[] = { "SIZE", "OPTIONAL", "MIN", "MAX", "DEFAULT", "WITH COMPONENT", "WITH COMPONENTS",
                             "INCLUDES", "ABSENT", "PRESENT", "PATTERN", "" };

AsnProposalBuiltinsProvider::AsnProposalBuiltinsProvider()
    : ProposalItemsProvider(":/codemodel/images/keyword.png")
{
}

QList<TextEditor::AssistProposalItemInterface *> AsnProposalBuiltinsProvider::createProposals() const
{
    QList<TextEditor::AssistProposalItemInterface *> proposals;

    proposals.append(createProposalsGroup(KEYWORDS));
    proposals.append(createProposalsGroup(TYPES));
    proposals.append(createProposalsGroup(BUILTIN));
    proposals.append(createProposalsGroup(ATTRIBUTES));

    return proposals;
}

QList<TextEditor::AssistProposalItemInterface *>
AsnProposalBuiltinsProvider::createProposalsGroup(const char *group[]) const
{
    QList<TextEditor::AssistProposalItemInterface *> proposals;

    int i = 0;
    while (group[i] != '\0') {
        addProposal(proposals, QLatin1String(group[i]));
        i++;
    }

    return proposals;
}

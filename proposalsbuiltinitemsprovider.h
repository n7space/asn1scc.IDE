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

#pragma once

#include <QList>

#include <texteditor/codeassist/assistproposalitem.h>

#include "proposalitemsprovider.h"

namespace Asn1Acn {
namespace Internal {

class ProposalBuiltinsProvider : public ProposalItemsProvider
{
public:
    ProposalBuiltinsProvider(const char *keywords[],
                             const char *types[],
                             const char *builtin[],
                             const char *attributes[]);

private:
    QList<TextEditor::AssistProposalItemInterface *> createProposals() const override;
    QList<TextEditor::AssistProposalItemInterface *> createProposalsGroup(const char *proposalsGroup[]) const;

    const char **m_keywords;
    const char **m_types;
    const char **m_builtin;
    const char **m_attributes;
};

} /* namespace Internal */
} /* namespace Asn1Acn */

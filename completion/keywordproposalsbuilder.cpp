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
#include <QString>
#include <QStringList>

#include "keywordproposalsbuilder.h"

using namespace Asn1Acn::Internal::Completion;

KeywordProposalsBuilder::KeywordProposalsBuilder(const QStringList &keywords,
                                                 const QStringList &types,
                                                 const QStringList &builtin,
                                                 const QStringList &attributes)
    : ProposalsBuilder(":/codemodel/images/keyword.png")
    , m_keywords(keywords)
    , m_types(types)
    , m_builtin(builtin)
    , m_attributes(attributes)
{
}

void KeywordProposalsBuilder::fillProposals()
{
    appendProposalsGroup(m_keywords);
    appendProposalsGroup(m_types);
    appendProposalsGroup(m_builtin);
    appendProposalsGroup(m_attributes);
}

void KeywordProposalsBuilder::appendProposalsGroup(const QStringList &group)
{
    foreach(const QString &word, group)
        addProposal(word);
}

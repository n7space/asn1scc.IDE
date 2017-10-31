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
#include "keywordproposalsbuilder.h"

#include <QString>
#include <QStringList>

#include <utils/icon.h>

using namespace Asn1Acn::Internal::Completion;

KeywordProposalsBuilder::KeywordProposalsBuilder(const QStringList &keywords,
                                                 const QStringList &types,
                                                 const QStringList &builtin,
                                                 const QStringList &attributes)
    : m_keywords(keywords)
    , m_types(types)
    , m_builtin(builtin)
    , m_attributes(attributes)
{
}

static QIcon buildIcon(Utils::Theme::Color theme)
{
    return Utils::Icon({{QLatin1String(":/codemodel/images/keyword.png"),
                         theme}}, Utils::Icon::Tint).icon();
}

void KeywordProposalsBuilder::fillProposals()
{
    const static QIcon keywordIcon(buildIcon(Utils::Theme::IconsCodeModelKeywordColor));
    const static QIcon typesIcon(buildIcon(Utils::Theme::IconsCodeModelClassColor));
    const static QIcon builtinIcon(buildIcon(Utils::Theme::IconsCodeModelMacroColor));
    const static QIcon attributeIcon(buildIcon(Utils::Theme::IconsCodeModelAttributeColor));

    appendProposalsGroup(m_keywords, keywordIcon);
    appendProposalsGroup(m_types, typesIcon);
    appendProposalsGroup(m_builtin, builtinIcon);
    appendProposalsGroup(m_attributes, attributeIcon);
}

void KeywordProposalsBuilder::appendProposalsGroup(const QStringList &group, const QIcon &icon)
{
    foreach(const QString &word, group)
        addProposal(word, icon);
}

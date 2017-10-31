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
#include "asnkeywordproposalsbuilder.h"

#include <QStringList>

using namespace Asn1Acn::Internal::Completion;

static const QStringList KEYWORDS = { "PLUS-INFINITY", "MINUS-INFINITY", "AUTOMATIC", "TAGS", "EXPLICIT", "IMPLICIT",
                                      "BEGIN", "END", "ALL EXCEPT", "EXPORTS", "DEFINITIONS", "APPLICATION", "PRIVATE",
                                      "UNIVERSAL", "EXCEPT", "UNION", "INTERSECTION FROM" };

static const QStringList TYPES = { "ENUMERATED", "INTEGER", "REAL", "BOOLEAN", "CHOICE", "SET", "SEQUENCE",
                                   "OCTET STRING", "BIT STRING" };

static const QStringList BUILTIN = { "NULL", "FALSE", "TRUE" };

static const QStringList ATTRIBUTES = { "SIZE", "OPTIONAL", "MIN", "MAX", "DEFAULT", "WITH COMPONENT", "WITH COMPONENTS",
                                        "INCLUDES", "ABSENT", "PRESENT", "PATTERN" };

AsnKeywordProposalsBuilder::AsnKeywordProposalsBuilder()
    : KeywordProposalsBuilder(KEYWORDS, TYPES, BUILTIN, ATTRIBUTES)
{
}

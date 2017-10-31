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
#include "acnkeywordproposalsbuilder.h"

#include <QStringList>

using namespace Asn1Acn::Internal::Completion;

static const QStringList KEYWORDS = { "BEGIN", "END", "DEFINITIONS", "CONSTANT" };

static const QStringList TYPES = { "INTEGER", "BOOLEAN" };

static const QStringList BUILTIN = { "NULL", "big", "little", "pos-int", "twos-complement", "BCD", "ASCII",
                                     "IEEE754-1985-32", "IEEE754-1985-64", "byte", "word", "dword" };

static const QStringList ATTRIBUTES = { "endianness", "encoding", "size", "null-terminated", "termination-pattern",
                                        "align-to-next", "encode-values", "true-value", "false-value", "pattern",
                                        "mapping-function", "present-when", "determinant" };

AcnKeywordProposalsBuilder::AcnKeywordProposalsBuilder()
    : KeywordProposalsBuilder(KEYWORDS, TYPES, BUILTIN, ATTRIBUTES)
{
}

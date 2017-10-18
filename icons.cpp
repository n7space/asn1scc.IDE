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
#include "icons.h"

#include <QString>

#include <utils/stylehelper.h>

using namespace Asn1Acn::Internal;

namespace
{
QString baseIconFileForType(const Data::Type type)
{
    switch (type.m_kind) {
    case Data::Type::Kind::Boolean:       return QStringLiteral(":/asn1acn/images/outline/boolean.png");
    case Data::Type::Kind::Null:          return QStringLiteral(":/asn1acn/images/outline/null.png");
    case Data::Type::Kind::Integer:       return QStringLiteral(":/asn1acn/images/outline/integer.png");
    case Data::Type::Kind::Real:          return QStringLiteral(":/asn1acn/images/outline/real.png");
    case Data::Type::Kind::BitString:     return QStringLiteral(":/asn1acn/images/outline/bitstring.png");
    case Data::Type::Kind::OctetString:   return QStringLiteral(":/asn1acn/images/outline/octetstring.png");
    case Data::Type::Kind::IA5String:     return QStringLiteral(":/asn1acn/images/outline/ia5string.png");
    case Data::Type::Kind::NumericString: return QStringLiteral(":/asn1acn/images/outline/numericstring.png");
    case Data::Type::Kind::Enumerated:    return QStringLiteral(":/asn1acn/images/outline/enumerated.png");
    case Data::Type::Kind::Choice:        return QStringLiteral(":/asn1acn/images/outline/choice.png");
    case Data::Type::Kind::Sequence:      return QStringLiteral(":/asn1acn/images/outline/sequence.png");
    case Data::Type::Kind::SequenceOf:    return QStringLiteral(":/asn1acn/images/outline/sequenceof.png");
    case Data::Type::Kind::UserDefined:   return QStringLiteral(":/asn1acn/images/outline/userdefined.png");
    }
    return QString();
}
} // namespace

QIcon Asn1Acn::Internal::Icons::iconForType(const Data::Type type)
{
    return QIcon(Utils::StyleHelper::dpiSpecificImageFile(baseIconFileForType(type)));
}

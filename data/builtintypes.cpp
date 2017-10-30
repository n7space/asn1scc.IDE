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

#include "builtintypes.h"

using namespace Asn1Acn::Internal::Data;

std::unique_ptr<Type> BuiltinType::createBuiltinType(const QString &name)
{
    if (name == QStringLiteral("BooleanType"))
        return std::make_unique<Boolean>();

    if (name == QStringLiteral("NullType"))
        return std::make_unique<Null>();

    if (name == QStringLiteral("IntegerType"))
        return std::make_unique<Integer>();

    if (name == QStringLiteral("RealType"))
        return std::make_unique<Real>();

    if (name == QStringLiteral("BitStringType"))
        return std::make_unique<BitString>();

    if (name == QStringLiteral("OctetStringType"))
        return std::make_unique<OctetString>();

    if (name == QStringLiteral("IA5StringType"))
        return std::make_unique<IA5String>();

    if (name == QStringLiteral("NumericStringType"))
        return std::make_unique<NumericString>();

    if (name == QStringLiteral("EnumeratedType"))
        return std::make_unique<Enumarated>();

    if (name == QStringLiteral("ChoiceType"))
        return std::make_unique<Choice>();

    if (name == QStringLiteral("SequenceType"))
        return std::make_unique<Sequence>();

    if (name == QStringLiteral("SequenceOfType"))
        return std::make_unique<SequenceOf>();

    return nullptr;
}

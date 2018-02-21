/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
** Contact: http://n7space.com
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

#include <memory>

#include <QString>

#include "type.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {
namespace Types {

class BuiltinType : public Type
{
public:
    static std::unique_ptr<Type> createBuiltinType(const QString &name);

    QString label() const override { return QLatin1String(": ") + name(); }
};

class Boolean : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("BOOLEAN"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/boolean.png");
    }
};

class Null : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("NULL"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/null.png");
    }
};

class Integer : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("INTEGER"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/integer.png");
    }
};

class Real : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("REAL"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/real.png");
    }
};

class BitString : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("BIT STRING"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/bitstring.png");
    }
};

class OctetString : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("OCTET STRING"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/octetstring.png");
    }
};

class IA5String : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("IA5String"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/ia5string.png");
    }
};

class NumericString : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("NumericString"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/numericstring.png");
    }
};

class Enumarated : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("ENUMERATED"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/enumerated.png");
    }
};

class Choice : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("CHOICE"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/choice.png");
    }
};

class Sequence : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("SEQUENCE"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/sequence.png");
    }
};

class SequenceOf : public BuiltinType
{
public:
    QString name() const override { return QLatin1String("SEQUENCE OF"); }

private:
    QString baseIconFile() const override
    {
        return QStringLiteral(":/asn1acn/images/outline/sequenceof.png");
    }
};

} // namespace Types
} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

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

#include "builtintype.h"

#include <map>

using namespace Asn1Acn::Internal::Data;

BuiltinType::BuiltinType(const QString &name)
    : Type(name)
{}

QString BuiltinType::baseIconFile() const
{
    static const std::map<QString, QString> iconsPaths = {
        { QStringLiteral("BOOLEAN"),       QStringLiteral(":/asn1acn/images/outline/boolean.png")       },
        { QStringLiteral("NULL"),          QStringLiteral(":/asn1acn/images/outline/null.png")          },
        { QStringLiteral("INTEGER"),       QStringLiteral(":/asn1acn/images/outline/integer.png")       },
        { QStringLiteral("REAL"),          QStringLiteral(":/asn1acn/images/outline/real.png")          },
        { QStringLiteral("BIT STRING"),    QStringLiteral(":/asn1acn/images/outline/bitstring.png")     },
        { QStringLiteral("OCTET STRING"),  QStringLiteral(":/asn1acn/images/outline/octetstring.png")   },
        { QStringLiteral("IA5String"),     QStringLiteral(":/asn1acn/images/outline/ia5string.png")     },
        { QStringLiteral("NumericString"), QStringLiteral(":/asn1acn/images/outline/numericstring.png") },
        { QStringLiteral("ENUMERATED"),    QStringLiteral(":/asn1acn/images/outline/enumerated.png")    },
        { QStringLiteral("CHOICE"),        QStringLiteral(":/asn1acn/images/outline/choice.png")        },
        { QStringLiteral("SEQUENCE"),      QStringLiteral(":/asn1acn/images/outline/sequence.png")      },
        { QStringLiteral("SEQUENCE OF"),   QStringLiteral(":/asn1acn/images/outline/sequenceof.png")    },
    };

    const auto it = iconsPaths.find(m_name);
    if (it == iconsPaths.end())
        return {};
    return it->second;
}

QString BuiltinType::label() const
{
    return ": " + m_name;
}

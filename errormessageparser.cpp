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
#include "errormessageparser.h"

#include <QRegularExpression>

using namespace Asn1Acn::Internal;

ErrorMessageParser::ErrorMessageParser(const QMap<QString, QString> &pathMapping)
    : m_pathMapping(pathMapping)
{
}

Data::ErrorMessage ErrorMessageParser::parse(const QString &message) const
{
    static const QRegularExpression regExp(R"(^(.+?):(\d+)(?::(\d+))?: error: (.*)$)");

    const auto match = regExp.match(message);
    if (!match.hasMatch())
        return {};

    const auto loc = Data::SourceLocation(mapPath(match.captured(1)), match.captured(2).toInt(), match.captured(3).toInt());
    return {loc, match.captured(4)};
}

QString ErrorMessageParser::mapPath(const QString &path) const
{
    return m_pathMapping.value(path, path);
}

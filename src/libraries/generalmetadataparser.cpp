/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
** Contact: https://n7space.com
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
#include "generalmetadataparser.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>

using namespace Asn1Acn::Internal::Libraries;

GeneralMetadataParser::GeneralMetadataParser(const QByteArray &data, const QString &path)
    : m_document(QJsonDocument::fromJson(data))
    , m_path(path)
{}

QString GeneralMetadataParser::readName(const QJsonObject &object)
{
    const auto name = object["name"].toString();
    if (name.isEmpty())
        return QFileInfo(m_path).baseName();
    return name;
}

namespace {
void assignExtraProperties(Metadata::General &res, const QJsonObject &object)
{
    res.setDescription(object["description"].toString());
    res.setLicense(object["license"].toString());
    res.setVersion(object["version"].toString());
    res.setVendor(object["vendor"].toString());
}
} // namespace

Metadata::General GeneralMetadataParser::parse()
{
    const auto obj = m_document.object();
    auto res = Metadata::General(readName(obj), m_path);
    if (!m_document.isNull())
        assignExtraProperties(res, obj);
    return res;
}

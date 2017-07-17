/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

#include "parseddocumentbuilder.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <extensionsystem/pluginmanager.h>

#include "astxmlparser.h"

#include "data/modules.h"
#include "data/definitions.h"

#include "documentsourceinfo.h"

using namespace Asn1Acn::Internal;

ParsedDocumentBuilder::ParsedDocumentBuilder(const QHash<QString, DocumentSourceInfo> &documents) :
    m_rawDocuments(documents)
{
    auto serviceProvider = ExtensionSystem::PluginManager::getObject<Asn1SccServiceProvider>();

    QNetworkReply *reply = serviceProvider->requestAst(documents);
    QObject::connect(reply, &QNetworkReply::finished,
                     this, &ParsedDocumentBuilder::requestFinished);
}

void ParsedDocumentBuilder::requestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() == QNetworkReply::NoError)
        parseResponse(reply->readAll());

    reply->deleteLater();
}

void ParsedDocumentBuilder::parseResponse(const QByteArray &jsonData)
{
    const auto json = QJsonDocument::fromJson(jsonData).object();
    if (responseContainsAst(json)) {
        parseXML(getAstXml(json));
        emit finished();
    }
    else {
        storeErrorMessages(json);
        emit errored();
    }
}

void ParsedDocumentBuilder::parseXML(const QString &textData)
{
    QXmlStreamReader reader;
    reader.addData(textData);

    AstXmlParser parser(reader);
    parser.parse();

    std::map<QString, std::unique_ptr<Data::Modules> > parsedData = parser.takeData();
    std::map<QString, std::unique_ptr<Data::Modules> >::iterator it;

    for (it = parsedData.begin(); it != parsedData.end(); it++) {
        QString fileName = it->first;
        DocumentSourceInfo sourceInfo = m_rawDocuments[fileName];

        std::unique_ptr<ParsedDocument> parsedDoc(new ParsedDocument(std::move(it->second), sourceInfo));

        m_parsedDocuments.push_back(std::move(parsedDoc));
    }
}

std::vector<std::unique_ptr<ParsedDocument> > ParsedDocumentBuilder::takeDocuments()
{
    return std::move(m_parsedDocuments);
}

bool ParsedDocumentBuilder::responseContainsAst(const QJsonObject &json)
{
    return json[QLatin1Literal("ErrorCode")].toInt(-1) == 0;
}

void ParsedDocumentBuilder::storeErrorMessages(const QJsonObject &json)
{
    for (const auto message : json[QLatin1Literal("Messages")].toArray())
        m_errorMessages.append(message.toString());
}

QString ParsedDocumentBuilder::getAstXml(const QJsonObject &json)
{
    const auto files = json[QLatin1Literal("Files")].toArray();
    if (files.size() != 1)
        return QString::null;
    return files[0].toObject()[QLatin1Literal("Contents")].toString();
}

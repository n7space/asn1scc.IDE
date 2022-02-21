/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
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

#include "asn1sccparseddocumentbuilder.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkReply>

#include <extensionsystem/pluginmanager.h>

#include <messages/messageutils.h>

#include "data/definitions.h"
#include "data/file.h"

#include "astxmlparser.h"
#include "errormessageparser.h"

using namespace Asn1Acn::Internal;

ParsedDocumentBuilder *Asn1SccParsedDocumentBuilder::create(const QHash<QString, QString> &documents)
{
    auto serviceProvider = ExtensionSystem::PluginManager::getObject<ParsingServiceProvider>();
    return new Asn1SccParsedDocumentBuilder(serviceProvider, documents);
}

Asn1SccParsedDocumentBuilder::Asn1SccParsedDocumentBuilder(ParsingServiceProvider *serviceProvider,
                                                           const QHash<QString, QString> &documents)
    : m_serviceProvider(serviceProvider)
    , m_documentSources(documents)
{}

void Asn1SccParsedDocumentBuilder::run()
{
    QNetworkReply *reply = m_serviceProvider->requestAst(m_documentSources);

    QObject::connect(reply,
                     &QNetworkReply::finished,
                     this,
                     &Asn1SccParsedDocumentBuilder::requestFinished);
}

void Asn1SccParsedDocumentBuilder::requestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        Messages::messageNetworkReply(reply);
        parseResponse(reply->readAll());
    } else {
        Messages::messageNetworkReplyError(reply);
        emit failed();
    }

    reply->deleteLater();
}

void Asn1SccParsedDocumentBuilder::parseResponse(const QByteArray &jsonData)
{
    const auto json = QJsonDocument::fromJson(jsonData).object();
    if (responseContainsAst(json)) {
        parseXML(getAstXml(json));
        emit finished();
    } else {
        storeErrorMessages(json);
        emit errored();
    }
}

void Asn1SccParsedDocumentBuilder::parseXML(const QString &textData)
{
    QXmlStreamReader reader;
    reader.addData(textData);

    AstXmlParser parser(reader);
    parser.parse();

    auto parsedData = parser.takeData();
    for (auto &item : parsedData)
        m_parsedDocuments.push_back(std::move(item.second));
}

std::vector<std::unique_ptr<Data::File>> Asn1SccParsedDocumentBuilder::takeDocuments()
{
    return std::move(m_parsedDocuments);
}

bool Asn1SccParsedDocumentBuilder::responseContainsAst(const QJsonObject &json)
{
    return json[QLatin1Literal("ErrorCode")].toInt(-1) == 0;
}

void Asn1SccParsedDocumentBuilder::storeErrorMessages(const QJsonObject &json)
{
    const auto parser = ErrorMessageParser();
    for (const auto message : json[QLatin1Literal("Messages")].toArray()) {
        const auto msg = parser.parse(message.toString());
        if (msg.isValid())
            m_errorMessages.push_back(msg);
    }
}

QString Asn1SccParsedDocumentBuilder::getAstXml(const QJsonObject &json)
{
    const auto files = json[QLatin1Literal("Files")].toArray();
    if (files.size() != 1)
        return QString();
    return files[0].toObject()[QLatin1Literal("Contents")].toString();
}

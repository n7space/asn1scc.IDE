/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
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

#pragma once

#include <memory>

#include <QByteArray>
#include <QHash>
#include <QString>

#include "asn1sccserviceprovider.h"
#include "parseddocumentbuilder.h"

class QJsonObject;

namespace Asn1Acn {
namespace Internal {

class Asn1SccParsedDocumentBuilder : public ParsedDocumentBuilder
{
    Q_OBJECT

public:
    static ParsedDocumentBuilder *create(const QHash<QString, QString> &documents);

    Asn1SccParsedDocumentBuilder(ParsingServiceProvider *serviceProvider,
                                 const QHash<QString, QString> &documents);
    void run() override;

    std::vector<std::unique_ptr<Data::File>> takeDocuments() override;
    const std::vector<Data::ErrorMessage> &errorMessages() const override
    {
        return m_errorMessages;
    }

private slots:
    void requestFinished();

private:
    void parseResponse(const QByteArray &jsonData);
    void parseXML(const QString &textData);
    void storeErrorMessages(const QJsonObject &json);

    bool responseContainsAst(const QJsonObject &json);
    QString getAstXml(const QJsonObject &json);

    ParsingServiceProvider *m_serviceProvider;

    const QHash<QString, QString> m_documentSources;

    std::vector<std::unique_ptr<Data::File>> m_parsedDocuments;
    std::vector<Data::ErrorMessage> m_errorMessages;
};

} /* namespace Internal */
} /* namespace Asn1Acn */

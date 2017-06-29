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

#include "documentprocessor.h"

#include <QTextCursor>
#include <QXmlStreamReader>
#include <QRegularExpression>

#include <QNetworkRequest>

#include <QJsonArray>
#include <QJsonObject>

#include <utils/fileutils.h>

#include "data/modules.h"
#include "data/definitions.h"

#include "parseddatastorage.h"
#include "astxmlparser.h"

using namespace Asn1Acn::Internal;

static const QString SEPARATOR_REG_EXP("\\s");
static const QString XML_VERSION_TAG("?xml version=\"1.0\"");
static const QString BASE_URL("http://localhost:9749/");

DocumentProcessor::DocumentProcessor(const QTextDocument *doc,
                                     const QString &filePath,
                                     int revision) :
    m_textDocument(doc),
    m_filePath(filePath),
    m_revision(revision)
{
}

void DocumentProcessor::run()
{
    ParsedDataStorage *model = ParsedDataStorage::instance();
    std::shared_ptr<ParsedDocument> oldDoc = model->getFileForPath(m_filePath);

    if (oldDoc && oldDoc->getRevision() == m_revision) {
        runFinished();
        return;
    }

    requestAst();
}

void DocumentProcessor::requestAst()
{
    QObject::connect(&m_networkAccessManager, &QNetworkAccessManager::finished,
                     this, &DocumentProcessor::requestFinished);

    auto astRequest = QNetworkRequest(QUrl(BASE_URL + "ast"));
    astRequest.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    QByteArray jsonRequestData = buildAstRequestData().toJson();
    m_networkAccessManager.post(astRequest, jsonRequestData);
}

QJsonDocument DocumentProcessor::buildAstRequestData() const
{
    QJsonObject asnFileData;

    // TODO: this should be based on m_filePath but server keeps rewriting the file if full valid path is passed.
    asnFileData["Name"] = "Test.asn";
    asnFileData["Contents"] = m_textDocument->toPlainText().toStdString().c_str();

    QJsonObject files;
    files["AsnFiles"] = QJsonArray { asnFileData };
    files["AcnFiles"] = QJsonArray();

    return QJsonDocument(files);
}

void DocumentProcessor::requestFinished(QNetworkReply *reply)
{
    const QByteArray &replyString = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning("Error Msg: %s", qUtf8Printable(QString::fromLatin1(replyString.data())));
        runFinished();
        return;
    }

    std::unique_ptr<ParsedDocument> newDoc = parseXML(replyString);

    ParsedDataStorage *model = ParsedDataStorage::instance();
    model->addFile(m_filePath, std::move(newDoc));

    runFinished();
}

std::unique_ptr<ParsedDocument> DocumentProcessor::parseXML(const QByteArray &textData) const
{    
    QXmlStreamReader reader;
    reader.addData(textData);

    AstXmlParser parser(reader);
    parser.parse();

    std::unique_ptr<Data::Modules> parsedData = parser.takeData();

    return std::unique_ptr<ParsedDocument>(new ParsedDocument(m_filePath,
                                                              m_textDocument->revision(),
                                                              std::move(parsedData)));
}

void DocumentProcessor::runFinished() const
{
    emit processingFinished();
}

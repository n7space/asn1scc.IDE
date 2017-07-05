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

#include <QFileInfo>
#include <QNetworkReply>
#include <QXmlStreamReader>

#include <extensionsystem/pluginmanager.h>

#include <utils/fileutils.h>

#include "data/modules.h"
#include "data/definitions.h"

#include "parseddatastorage.h"
#include "astxmlparser.h"

using namespace Asn1Acn::Internal;

DocumentProcessor::DocumentProcessor(const QTextDocument *doc,
                                     const QString &filePath,
                                     int revision) :
    m_textDocument(doc),
    m_filePath(filePath),
    m_revision(revision)
{
    m_serviceProvider = ExtensionSystem::PluginManager::getObject<Asn1SccServiceProvider>();
}

void DocumentProcessor::run()
{
    ParsedDataStorage *model = ParsedDataStorage::instance();
    std::shared_ptr<ParsedDocument> oldDoc = model->getFileForPath(m_filePath);

    if (oldDoc && oldDoc->getRevision() == m_revision) {
        runFinished();
        return;
    }

    QNetworkReply *reply = m_serviceProvider->requestAst(m_textDocument->toPlainText(), QFileInfo(m_filePath));

    QObject::connect(reply, &QNetworkReply::finished,
                     this, &DocumentProcessor::requestFinished);
}

void DocumentProcessor::requestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply != nullptr && reply->error() != QNetworkReply::NoError) {
        runFinished();
        return;
    }

    const QByteArray &replyString = reply->readAll();
    ParsedDataStorage *model = ParsedDataStorage::instance();

    std::unique_ptr<ParsedDocument> newDoc = parseXML(replyString);
    model->addFile(m_filePath, std::move(newDoc));

    reply->deleteLater();

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
                                                              m_revision,
                                                              std::move(parsedData)));
}

void DocumentProcessor::runFinished() const
{
    emit processingFinished();
}

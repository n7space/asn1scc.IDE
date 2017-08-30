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

#include "asn1sccdocumentprocessor.h"

#include <QFileInfo>

#include "parseddocument.h"
#include "asn1sccparseddocumentbuilder.h"

using namespace Asn1Acn::Internal;

Asn1SccDocumentProcessor *Asn1SccDocumentProcessor::create(const QString &projectName)
{
    ParsedDocumentBuilder *docBuilder = Asn1SccParsedDocumentBuilder::create();
    return new Asn1SccDocumentProcessor(projectName, docBuilder);
}

Asn1SccDocumentProcessor::Asn1SccDocumentProcessor(const QString &projectName, ParsedDocumentBuilder *docBuilder)
    : m_projectName(projectName),
      m_state(State::Unfinished),
      m_docBuilder(docBuilder)
{
    QObject::connect(dynamic_cast<QObject *>(m_docBuilder), SIGNAL(finished()), this, SLOT(onBuilderFinished()));
    QObject::connect(dynamic_cast<QObject *>(m_docBuilder), SIGNAL(failed()), this, SLOT(onBuilderFailed()));
    QObject::connect(dynamic_cast<QObject *>(m_docBuilder), SIGNAL(errored()), this, SLOT(onBuilderErrored()));
}

Asn1SccDocumentProcessor::~Asn1SccDocumentProcessor()
{
    delete m_docBuilder;
}

void Asn1SccDocumentProcessor::addToRun(const QString &docContent, const QString &filePath, int revision)
{
    QString fileName = QFileInfo(filePath).fileName();
    DocumentSourceInfo fileInfo(revision, docContent, filePath, fileName);

    m_documents.insert(fileName, fileInfo);
}

void Asn1SccDocumentProcessor::run()
{
    m_docBuilder->setDocumentsToProcess(&m_documents);
    m_docBuilder->run();
}

Asn1SccDocumentProcessor::State Asn1SccDocumentProcessor::getState()
{
    return m_state;
}

std::vector<std::unique_ptr<ParsedDocument>> Asn1SccDocumentProcessor::takeResults()
{
    return std::move(m_results);
}

void Asn1SccDocumentProcessor::onBuilderFinished()
{
    m_results = m_docBuilder->takeDocuments();
    m_state = State::Successful;

    emit processingFinished(m_projectName);
}

void Asn1SccDocumentProcessor::onBuilderFailed()
{
    createFallbackResults();
    m_state = State::Failed;

    emit processingFinished(m_projectName);
}

void Asn1SccDocumentProcessor::onBuilderErrored()
{
    createFallbackResults();
    m_state = State::Errored;

    emit processingFinished(m_projectName);
}

void Asn1SccDocumentProcessor::createFallbackResults()
{
    QHashIterator<QString, DocumentSourceInfo> it(m_documents);
    while (it.hasNext()) {
        it.next();
        m_results.push_back(std::make_unique<ParsedDocument>(it.value()));
    }
}

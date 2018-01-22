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

#include "asn1sccdocumentprocessor.h"

#include "asn1sccparseddocumentbuilder.h"

using namespace Asn1Acn::Internal;

Asn1SccDocumentProcessor *Asn1SccDocumentProcessor::create(const QString &projectName)
{
    return new Asn1SccDocumentProcessor(projectName,
                                        [](const QHash<QString, QString> &documents)->ParsedDocumentBuilder *
                                        { return Asn1SccParsedDocumentBuilder::create(documents); },
                                        ParsedDataStorage::instance());
}

Asn1SccDocumentProcessor::Asn1SccDocumentProcessor(const QString &projectName,
                                                   DocumentBuilderCreator docBuilderCreator,
                                                   ParsedDataStorage *storage)
    : m_projectName(projectName)
    , m_state(State::Unfinished)
    , m_docBuilder(nullptr)
    , m_docBuilderCreator(docBuilderCreator)
    , m_storage(storage)
{
    m_index = m_storage->getProjectBuildersCount(m_projectName) + 1;
    m_storage->setProjectBuildersCount(m_projectName, m_index);
}

void Asn1SccDocumentProcessor::addToRun(const QString &filePath, const QString &docContent)
{
    m_documents.insert(filePath, docContent);
}

void Asn1SccDocumentProcessor::run()
{
    m_docBuilder.reset(m_docBuilderCreator(m_documents));

    connect(m_docBuilder.get(), &ParsedDocumentBuilder::finished,
            this, &Asn1SccDocumentProcessor::onBuilderFinished);
    connect(m_docBuilder.get(), &ParsedDocumentBuilder::failed,
            this, &Asn1SccDocumentProcessor::onBuilderFailed);
    connect(m_docBuilder.get(), &ParsedDocumentBuilder::errored,
            this, &Asn1SccDocumentProcessor::onBuilderErrored);

    m_docBuilder->run();
}

Asn1SccDocumentProcessor::State Asn1SccDocumentProcessor::state()
{
    return m_state;
}

std::vector<std::unique_ptr<Data::File>> Asn1SccDocumentProcessor::takeResults()
{
    return std::move(m_results);
}

void Asn1SccDocumentProcessor::onBuilderFinished()
{
    m_results = m_docBuilder->takeDocuments();
    setState(State::Successful);

    emit processingFinished(m_projectName);
}

void Asn1SccDocumentProcessor::onBuilderFailed()
{
    createFallbackResults();
    setState(State::Failed);

    emit processingFinished(m_projectName);
}

void Asn1SccDocumentProcessor::onBuilderErrored()
{
    createFallbackResults();
    setState(State::Errored);

    emit processingFinished(m_projectName);
}

void Asn1SccDocumentProcessor::setState(Asn1SccDocumentProcessor::State expected)
{
    if (m_index < m_storage->getProjectBuildersCount(m_projectName))
        m_state = State::Outdated;
    else
        m_state = expected;
}

void Asn1SccDocumentProcessor::createFallbackResults()
{
    for (auto it = m_documents.begin(); it != m_documents.end(); it++)
        m_results.push_back(std::make_unique<Data::File>(it.key()));
}

const std::vector<Data::ErrorMessage> &Asn1SccDocumentProcessor::errorMessages() const
{
    return m_docBuilder->errorMessages();
}

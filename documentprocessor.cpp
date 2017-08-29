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

#include "documentprocessor.h"

#include <QFileInfo>

#include "parseddocument.h"
#include "parseddocumentbuilder.h"

using namespace Asn1Acn::Internal;

DocumentProcessor::DocumentProcessor(const QString &projectName) :
    m_projectName(projectName),
    m_state(State::Unfinished),
    m_docBuilder(nullptr)
{
}

DocumentProcessor::~DocumentProcessor()
{
    delete m_docBuilder;
}

void DocumentProcessor::addToRun(const QString &docContent, const QString &filePath, int revision)
{
    QString fileName = QFileInfo(filePath).fileName();
    DocumentSourceInfo fileInfo(revision, docContent, filePath, fileName);

    m_documents.insert(fileName, fileInfo);
}

void DocumentProcessor::run()
{
    m_docBuilder = ParsedDocumentBuilder::create(m_documents);
    m_docBuilder->run();

    QObject::connect(dynamic_cast<QObject *>(m_docBuilder), SIGNAL(finished()), this, SLOT(onBuilderFinished()));
    QObject::connect(dynamic_cast<QObject *>(m_docBuilder), SIGNAL(failed()), this, SLOT(onBuilderFailed()));
    QObject::connect(dynamic_cast<QObject *>(m_docBuilder), SIGNAL(errored()), this, SLOT(onBuilderErrored()));
}

DocumentProcessor::State DocumentProcessor::getState()
{
    return m_state;
}

std::vector<std::unique_ptr<ParsedDocument>> DocumentProcessor::takeResults()
{
    return std::move(m_results);
}

void DocumentProcessor::onBuilderFinished()
{
    m_results = m_docBuilder->takeDocuments();
    m_state = State::Successful;

    emit processingFinished(m_projectName);
}

void DocumentProcessor::onBuilderFailed()
{
    createFallbackResults();
    m_state = State::Failed;

    emit processingFinished(m_projectName);
}

void DocumentProcessor::onBuilderErrored()
{
    createFallbackResults();
    m_state = State::Errored;

    emit processingFinished(m_projectName);
}

void DocumentProcessor::createFallbackResults()
{
    QHashIterator<QString, DocumentSourceInfo> it(m_documents);
    while (it.hasNext()) {
        it.next();
        m_results.push_back(std::make_unique<ParsedDocument>(it.value()));
    }
}

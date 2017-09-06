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

#include "documentprocessorstub.h"

#include <QList>

#include "../data/modules.h"

using namespace Asn1Acn::Internal;

DocumentProcessorStub::DocumentProcessorStub(const QString &project)
    : m_projectName(project)
{
}

void DocumentProcessorStub::addToRun(const QString &docContent, const QString &filePath, int revision)
{
    QString fileName = QFileInfo(filePath).fileName();
    DocumentSourceInfo fileInfo(revision, docContent, filePath, fileName);

    m_documents.insert(fileName, fileInfo);

    if (docContent.contains("SUCCESS"))
        m_state = State::Successful;
    else if (docContent.contains("FAILED"))
        m_state = State::Failed;
    else if (docContent.contains("ERROR"))
        m_state = State::Errored;
}

void DocumentProcessorStub::run()
{
    for (auto it = m_documents.begin(); it != m_documents.cend(); ++it) {
        auto modules = std::make_unique<Data::Modules>();
        std::unique_ptr<ParsedDocument> parsedDocument(new ParsedDocument(std::move(modules), it.value()));
        m_results.push_back(std::move(parsedDocument));
    }

    emit processingFinished(m_projectName);
}

std::vector<std::unique_ptr<ParsedDocument>> DocumentProcessorStub::takeResults()
{
    return std::move(m_results);
}

DocumentProcessorStub::State DocumentProcessorStub::getState()
{
    return m_state;
}

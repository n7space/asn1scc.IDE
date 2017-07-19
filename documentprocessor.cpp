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

#include "parseddocument.h"

using namespace Asn1Acn::Internal;

DocumentProcessor::DocumentProcessor(const QString &projectName) :
    m_projectName(projectName),
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
    m_docBuilder = new ParsedDocumentBuilder(m_documents);

    QObject::connect(m_docBuilder, &ParsedDocumentBuilder::finished,
                     this, &DocumentProcessor::onBuilderFinished);

    QObject::connect(m_docBuilder, &ParsedDocumentBuilder::failed,
                     this, &DocumentProcessor::onBuilderFinished);

    // TODO: error should be handled exclusively
    QObject::connect(m_docBuilder, &ParsedDocumentBuilder::errored,
                     this, &DocumentProcessor::onBuilderFinished);
}

std::vector<std::unique_ptr<ParsedDocument>> DocumentProcessor::takeResults()
{
    return m_docBuilder != nullptr ? m_docBuilder->takeDocuments() : std::vector<std::unique_ptr<ParsedDocument>>();
}

void DocumentProcessor::onBuilderFinished()
{
    emit processingFinished(m_projectName);
}

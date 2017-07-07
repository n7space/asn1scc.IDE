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

#include <utils/fileutils.h>

#include "parseddocument.h"
#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;

DocumentProcessor::DocumentProcessor(const QTextDocument *doc,
                                     const QString &filePath,
                                     int revision) :
    m_textDocument(doc),
    m_filePath(filePath),
    m_revision(revision),
    m_docBuilder(nullptr)

{
}

DocumentProcessor::~DocumentProcessor()
{
    delete m_docBuilder;
}

void DocumentProcessor::run()
{
    ParsedDataStorage *model = ParsedDataStorage::instance();
    std::shared_ptr<ParsedDocument> oldDoc = model->getFileForPath(m_filePath);

    if (oldDoc && oldDoc->getRevision() == m_revision) {
        emit processingFinished();
        return;
    }

    m_docBuilder = new ParsedDocumentBuilder(m_textDocument->toPlainText(), QFileInfo(m_filePath), m_revision);

    QObject::connect(m_docBuilder, &ParsedDocumentBuilder::finished,
                     this, &DocumentProcessor::onBuilderFinished);
}

void DocumentProcessor::onBuilderFinished()
{
    std::unique_ptr<ParsedDocument> doc = m_docBuilder->takeDocument();
    if (doc != nullptr) {
        ParsedDataStorage *model = ParsedDataStorage::instance();
        model->addFile(m_filePath, std::move(doc));
    }

    emit processingFinished();
}

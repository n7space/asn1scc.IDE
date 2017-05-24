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

#include "asndocumentprocessor.h"

#include <QRegularExpression>

#include <utils/fileutils.h>

#include "asnparseddatastorage.h"

using namespace Asn1Acn::Internal;

const QString SEPARATOR_REG_EXP("\\s");

AsnDocumentProcessor::AsnDocumentProcessor(const TextEditor::TextDocument &doc) :
    m_textDocument(doc.document()),
    m_filePath((doc.filePath()).fileName())
{
}

void AsnDocumentProcessor::run() const
{
    AsnParsedDataStorage *model = AsnParsedDataStorage::instance();

    std::shared_ptr<AsnParsedDocument> doc = model->getDataForFile(m_filePath);
    if (!doc || doc->getRevision() != m_textDocument->revision()) {
        std::unique_ptr<AsnParsedDocument> asnFile(parse());
        model->update(m_filePath, std::move(asnFile));
    }

    // emit after parsing is finished
    emit asnDocumentUpdated(*m_textDocument);
}

std::unique_ptr<AsnParsedDocument> AsnDocumentProcessor::parse() const
{
    // In this place parsing procedure should be executed
    QString docPlainText = m_textDocument->toPlainText();
    QStringList splittedDoc = docPlainText.split(QRegularExpression(SEPARATOR_REG_EXP),
                                                 QString::SkipEmptyParts);

    return std::unique_ptr<AsnParsedDocument>(new AsnParsedDocument(m_filePath,
                                                                    m_textDocument->revision(),
                                                                    splittedDoc));
}

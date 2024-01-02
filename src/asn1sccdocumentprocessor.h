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

#include <functional>
#include <memory>

#include <QFileInfo>
#include <QHash>
#include <QString>
#include <QTextDocument>

#include "documentprocessor.h"
#include "parseddatastorage.h"
#include "parseddocumentbuilder.h"

namespace Asn1Acn {
namespace Internal {

class Asn1SccDocumentProcessor : public DocumentProcessor
{
    Q_OBJECT
public:
    using State = DocumentProcessor::State;
    using DocumentBuilderCreator
        = std::function<ParsedDocumentBuilder *(const QHash<QString, QString> &documents)>;

    static Asn1SccDocumentProcessor *create(const QString &projectName);

    Asn1SccDocumentProcessor(const QString &projectName,
                             DocumentBuilderCreator docBuilderCreator,
                             ParsedDataStorage *storage);

    void addToRun(const QString &filePath, const QString &docContent) override;
    void run() override;
    std::vector<std::unique_ptr<Data::File>> takeResults() override;
    const std::vector<Data::ErrorMessage> &errorMessages() const override;

    State state() override;

private slots:
    void onBuilderFinished();
    void onBuilderFailed();
    void onBuilderErrored();

private:
    void createFallbackResults();
    void setState(Asn1SccDocumentProcessor::State expected);

    QHash<QString, QString> m_documents;
    QString m_projectName;

    std::vector<std::unique_ptr<Data::File>> m_results;
    State m_state;
    int m_index;

    std::unique_ptr<ParsedDocumentBuilder> m_docBuilder;
    DocumentBuilderCreator m_docBuilderCreator;

    ParsedDataStorage *m_storage;
};

} // namespace Internal
} // namespace Asn1Acn

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

#pragma once

#include <functional>
#include <memory>

#include <QHash>
#include <QString>
#include <QFileInfo>
#include <QTextDocument>

#include "data/source.h"

#include "parseddocumentbuilder.h"
#include "documentprocessor.h"

namespace Asn1Acn {
namespace Internal {

class Asn1SccDocumentProcessor
        : public DocumentProcessor
{
    Q_OBJECT
public:

    using State = DocumentProcessor::State;
    using DocumentBuilderCreator = std::function<ParsedDocumentBuilder *(const QHash<QString, QString> &documents)>;

    static Asn1SccDocumentProcessor *create(const QString &projectName);

    Asn1SccDocumentProcessor(const QString &projectName, DocumentBuilderCreator docBuilderCreator);
    ~Asn1SccDocumentProcessor();

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

    QHash<QString, QString> m_documents;
    QString m_projectName;

    std::vector<std::unique_ptr<Data::File>> m_results;
    State m_state;

    std::unique_ptr<ParsedDocumentBuilder> m_docBuilder;
    DocumentBuilderCreator m_docBuilderCreator;
};

} // namespace Internal
} // namespace Asn1Acn

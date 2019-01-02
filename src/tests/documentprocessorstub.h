/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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

#pragma once

#include <memory>
#include <vector>

#include <QHash>
#include <QString>

#include <documentprocessor.h>

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class DocumentProcessorStub : public DocumentProcessor
{
    Q_OBJECT
public:
    using State = DocumentProcessor::State;

    DocumentProcessorStub(const QString &project = QString());
    ~DocumentProcessorStub() = default;

    void addToRun(const QString &filePath, const QString &docContent) override;
    void run() override;
    std::vector<std::unique_ptr<Data::File>> takeResults() override;
    const std::vector<Data::ErrorMessage> &errorMessages() const override;

    State state() override;

private:
    State createState();

    State m_state;
    QString m_projectName;

    QHash<QString, QString> m_documents;
    std::vector<std::unique_ptr<Data::File>> m_results;
    std::vector<Data::ErrorMessage> m_errorMessages;
};

} // namespace Tests
} // namespace Internal
} // namespace Asn1Acn

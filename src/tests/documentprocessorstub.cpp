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

#include "documentprocessorstub.h"

#include <QFileInfo>
#include <QList>

#include <data/file.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

DocumentProcessorStub::DocumentProcessorStub(const QString &project)
    : m_state(State::Unfinished)
    , m_projectName(project)
{}

void DocumentProcessorStub::addToRun(const QString &filePath, const QString &docContent)
{
    m_documents.insert(filePath, docContent);
}

void DocumentProcessorStub::run()
{
    m_state = createState();

    for (auto it = m_documents.begin(); it != m_documents.end(); it++) {
        auto modules = std::make_unique<Data::File>(it.key());
        m_results.push_back(std::move(modules));
    }

    emit processingFinished(m_projectName);
}

std::vector<std::unique_ptr<Data::File>> DocumentProcessorStub::takeResults()
{
    return std::move(m_results);
}

DocumentProcessorStub::State DocumentProcessorStub::state()
{
    return m_state;
}

const std::vector<Data::ErrorMessage> &DocumentProcessorStub::errorMessages() const
{
    return m_errorMessages;
}

DocumentProcessorStub::State DocumentProcessorStub::createState()
{
    if (m_documents.empty())
        return State::Successful;

    const auto content = m_documents.begin().value();

    if (content == QStringLiteral("SUCCESS"))
        return State::Successful;

    if (content == QStringLiteral("FAILED"))
        return State::Failed;

    if (content == QStringLiteral("ERROR"))
        return State::Errored;

    return State::Errored;
}

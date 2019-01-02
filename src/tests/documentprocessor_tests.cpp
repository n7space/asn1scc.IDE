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

#include "documentprocessor_tests.h"

#include <QtTest>

#include <data/project.h>
#include <parseddatastorage.h>

#include "parseddocumentbuilderstub.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

DocumentProcessorTests::DocumentProcessorTests(QObject *parent)
    : QObject(parent)
    , m_fileDir("/test/dir/")
    , m_docBuilderCreator([](const QHash<QString, QString> &documents) -> ParsedDocumentBuilder * {
        return new ParsedDocumentBuilderStub(documents);
    })
    , m_storage(std::make_unique<ParsedDataStorage>())
{}

void DocumentProcessorTests::test_unstarted()
{
    DocumentProcessor *dp = new Asn1SccDocumentProcessor("ProjectName",
                                                         m_docBuilderCreator,
                                                         m_storage.get());

    QCOMPARE(dp->state(), DocumentProcessor::State::Unfinished);

    const std::vector<std::unique_ptr<Data::File>> results = dp->takeResults();
    QCOMPARE(results.size(), static_cast<size_t>(0));

    delete dp;
}

void DocumentProcessorTests::test_successful()
{
    const QString fileName("SUCCESS");
    const QString content("SUCCESS");
    const QString filePath = m_fileDir + fileName;

    DocumentProcessor *dp = new Asn1SccDocumentProcessor(m_projectName,
                                                         m_docBuilderCreator,
                                                         m_storage.get());
    QSignalSpy spy(dp, &DocumentProcessor::processingFinished);

    dp->addToRun(filePath, content);
    dp->run();

    examine(dp, spy, DocumentProcessor::State::Successful, fileName, filePath);

    delete dp;
}

void DocumentProcessorTests::test_error()
{
    const QString fileName("ERROR");
    const QString content("ERROR");
    const QString filePath = m_fileDir + fileName;

    DocumentProcessor *dp = new Asn1SccDocumentProcessor(m_projectName,
                                                         m_docBuilderCreator,
                                                         m_storage.get());
    QSignalSpy spy(dp, &DocumentProcessor::processingFinished);

    dp->addToRun(filePath, content);
    dp->run();

    examine(dp, spy, DocumentProcessor::State::Errored, fileName, filePath);

    delete dp;
}

void DocumentProcessorTests::test_failed()
{
    const QString fileName("FAILED");
    const QString content("FAILED");
    const QString filePath = m_fileDir + fileName;

    DocumentProcessor *dp = new Asn1SccDocumentProcessor(m_projectName,
                                                         m_docBuilderCreator,
                                                         m_storage.get());
    QSignalSpy spy(dp, &DocumentProcessor::processingFinished);

    dp->addToRun(filePath, content);
    dp->run();

    examine(dp, spy, DocumentProcessor::State::Failed, fileName, filePath);

    delete dp;
}

void DocumentProcessorTests::test_multipleProcessors()
{
    const QString fileName("SUCCESS");
    const QString content("SUCCESS");
    const QString filePath = m_fileDir + fileName;

    m_storage->addProject(m_projectName);

    const int processorsCnt = 10;
    std::vector<DocumentProcessor *> processors;
    std::vector<QSignalSpy *> spies;

    for (int i = 0; i < processorsCnt; i++) {
        DocumentProcessor *dp = new Asn1SccDocumentProcessor(m_projectName,
                                                             m_docBuilderCreator,
                                                             m_storage.get());
        dp->addToRun(filePath, content);
        spies.push_back(new QSignalSpy(dp, &DocumentProcessor::processingFinished));

        processors.push_back(dp);
    }

    for (int i = 0; i < processorsCnt; i++)
        processors[i]->run();

    for (int i = 0; i < processorsCnt - 1; i++)
        examine(processors[i], *spies[i], DocumentProcessor::State::Outdated, fileName, filePath);

    examine(processors[processorsCnt - 1],
            *spies[processorsCnt - 1],
            DocumentProcessor::State::Successful,
            fileName,
            filePath);

    for (int i = 0; i < processorsCnt; i++) {
        delete processors[i];
        delete spies[i];
    }
}

void DocumentProcessorTests::examine(DocumentProcessor *dp,
                                     const QSignalSpy &spy,
                                     const DocumentProcessor::State state,
                                     const QString &fileName,
                                     const QString &filePath) const
{
    QCOMPARE(dp->state(), state);
    QCOMPARE(spy.count(), 1);

    const QVariant signalArg = spy.at(0).at(0);
    QCOMPARE(signalArg.type(), QVariant::String);
    QCOMPARE(qvariant_cast<QString>(signalArg), m_projectName);

    const std::vector<std::unique_ptr<Data::File>> results = dp->takeResults();
    QCOMPARE(results.size(), static_cast<size_t>(1));

    QCOMPARE(fileName, results.at(0)->location().fileName());
    QCOMPARE(filePath, results.at(0)->location().path());
}

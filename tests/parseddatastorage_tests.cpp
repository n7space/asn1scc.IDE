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

#include "parseddatastorage_tests.h"

#include <memory>

#include <QtTest>

#include "../parseddocument.h"
#include "../documentsourceinfo.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

ParsedDataStorageTests::ParsedDataStorageTests(QObject *parent)
    : QObject(parent)
{
    m_storage = ParsedDataStorageProxy::create();
}

ParsedDataStorageTests::~ParsedDataStorageTests()
{
    ParsedDataStorageProxy::finish(m_storage);
}

void ParsedDataStorageTests::test_addAndRemoveProject()
{
    const QString project("testProject");

    ParsedDataStorageProxy::addProject(m_storage, project);
    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 1);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 0);

    ParsedDataStorageProxy::removeProject(m_storage, project);
    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 0);
}

void ParsedDataStorageTests::test_addAndRemoveMultipleProjects()
{
    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    ParsedDataStorageProxy::addProject(m_storage, firstProject);
    ParsedDataStorageProxy::addProject(m_storage, secondProject);

    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 2);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 0);

    ParsedDataStorageProxy::removeProject(m_storage, firstProject);
    ParsedDataStorageProxy::removeProject(m_storage, secondProject);

    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 0);
}

void ParsedDataStorageTests::test_addAndRemoveFileFromProject()
{
    const QString project("testProject");
    ParsedDataStorageProxy::addProject(m_storage, project);

    const QString fileName("testFile");
    const QString path = pathFromName(fileName);

    addFileToProject(project, "content", fileName, path, 1);
    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 1);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 1);

    auto document = m_storage->getFileForPath(path);
    QVERIFY(document != nullptr);

    auto projects = m_storage->getProjectsForFile(path);
    QVERIFY(projects.contains(project));

    auto files = m_storage->getFilesFromProject(project);
    QCOMPARE(files.size(), 1);

    ParsedDataStorageProxy::removeFileFromProject(m_storage, project, path);

    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 0);

    document = m_storage->getFileForPath(path);
    QVERIFY(document == nullptr);

    projects = m_storage->getProjectsForFile(path);
    QVERIFY(projects.empty());

    files = m_storage->getFilesFromProject(project);
    QCOMPARE(files.size(), 0);

    ParsedDataStorageProxy::removeProject(m_storage, project);
    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 0);
}

void ParsedDataStorageTests::test_addAndRemoveMultipleFilesFromProject()
{
    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    ParsedDataStorageProxy::addProject(m_storage, firstProject);
    ParsedDataStorageProxy::addProject(m_storage, secondProject);
    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 2);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 0);


    const QString fileName("testFile");
    const QString path = pathFromName(fileName);
    addFileToProject(firstProject, "content", fileName, path, 1);
    addFileToProject(secondProject, "content", fileName, path, 1);

    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 2);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 1);

    auto projects = m_storage->getProjectsForFile(path);
    QCOMPARE(projects.size(), 2);

    auto files = m_storage->getFilesFromProject(firstProject);
    QCOMPARE(files.size(), 1);

    files = m_storage->getFilesFromProject(secondProject);
    QCOMPARE(files.size(), 1);

    ParsedDataStorageProxy::removeProject(m_storage, firstProject);
    ParsedDataStorageProxy::removeProject(m_storage, secondProject);
}

void ParsedDataStorageTests::test_removeNonemptyProject()
{
    const QString project("testProject");
    ParsedDataStorageProxy::addProject(m_storage, project);

    const QString fileName("testFile");
    addFileToProject(project, "content", fileName, pathFromName(fileName), 1);

    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 1);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 1);

    ParsedDataStorageProxy::removeProject(m_storage, project);

    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 0);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 0);
}

void ParsedDataStorageTests::test_updateFileInOneProject()
{
    const QString project("testProject");
    ParsedDataStorageProxy::addProject(m_storage, project);

    const QString fileName("testFile");
    addFileToProject(project, "content", fileName, pathFromName(fileName), 1);

    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 1);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 1);

    const QString refreshedContent = "changed content";
    addFileToProject(project, refreshedContent, fileName, pathFromName(fileName), 1);

    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 1);

    const QList<std::shared_ptr<ParsedDocument>> files = m_storage->getFilesFromProject(project);

    QCOMPARE(files.size(), 1);
    const DocumentSourceInfo received = files.at(0)->source();

    QCOMPARE(received.getContent(), refreshedContent);
    ParsedDataStorageProxy::removeProject(m_storage, project);
}

void ParsedDataStorageTests::test_updateFileInMultipleProjects()
{
    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    ParsedDataStorageProxy::addProject(m_storage, firstProject);
    ParsedDataStorageProxy::addProject(m_storage, secondProject);

    QCOMPARE(ParsedDataStorageProxy::projectsCnt(m_storage), 2);
    QCOMPARE(ParsedDataStorageProxy::filesCnt(m_storage), 0);

    const QString fileName("testFile");
    const QString path = pathFromName(fileName);
    addFileToProject(firstProject, "content", fileName, path, 1);
    addFileToProject(secondProject, "content", fileName, path, 1);

    const QString refreshedContent = "changed content";
    addFileToProject(firstProject, refreshedContent, fileName, path, 1);

    QList<std::shared_ptr<ParsedDocument>> files = m_storage->getFilesFromProject(firstProject);
    QCOMPARE(files.size(), 1);

    const DocumentSourceInfo received = files.at(0)->source();
    QCOMPARE(received.getContent(), refreshedContent);

    files = m_storage->getFilesFromProject(secondProject);
    QCOMPARE(files.size(), 1);
    DocumentSourceInfo receivedSecond = files.at(0)->source();

    QCOMPARE(receivedSecond.getContent(), refreshedContent);

    ParsedDataStorageProxy::removeProject(m_storage, firstProject);
    ParsedDataStorageProxy::removeProject(m_storage, secondProject);
}

void ParsedDataStorageTests::addFileToProject(const QString &project,
                                              const QString &fileContent,
                                              const QString &fileName,
                                              const QString &filePath,
                                              const int &revision)
{
    DocumentSourceInfo info(revision, fileContent, filePath, fileName);
    std::unique_ptr<ParsedDocument> parsedDocument(new ParsedDocument(std::make_unique<Data::Modules>(), info));
    ParsedDataStorageProxy::addFileToProject(m_storage, project, std::move(parsedDocument));
}

QString ParsedDataStorageTests::pathFromName(const QString &name)
{
    return QString("/test/file/path/") + name;
}

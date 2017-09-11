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
#include "../documentsource.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

ParsedDataStorageTests::ParsedDataStorageTests(QObject *parent)
    : QObject(parent)
{
}

void ParsedDataStorageTests::test_addAndRemoveProject()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QString project("testProject");

    ParsedDataStorageProxy::addProject(storage, project);
    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 1);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 0);

    ParsedDataStorageProxy::removeProject(storage, project);
    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 0);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_addAndRemoveMultipleProjects()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    ParsedDataStorageProxy::addProject(storage, firstProject);
    ParsedDataStorageProxy::addProject(storage, secondProject);

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 2);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 0);

    ParsedDataStorageProxy::removeProject(storage, firstProject);
    ParsedDataStorageProxy::removeProject(storage, secondProject);

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 0);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_addAndRemoveFileFromProject()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QString project("testProject");
    ParsedDataStorageProxy::addProject(storage, project);

    const QString path = pathFromName("testFile");

    addFileToProject(storage, project, "content", path);
    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 1);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 1);

    auto document = storage->getFileForPath(path);
    QVERIFY(document != nullptr);

    auto projects = storage->getProjectsForFile(path);
    QVERIFY(projects.contains(project));

    auto files = storage->getFilesFromProject(project);
    QCOMPARE(files.size(), 1);

    ParsedDataStorageProxy::removeFileFromProject(storage, project, path);

    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 0);

    document = storage->getFileForPath(path);
    QVERIFY(document == nullptr);

    projects = storage->getProjectsForFile(path);
    QVERIFY(projects.empty());

    files = storage->getFilesFromProject(project);
    QCOMPARE(files.size(), 0);

    ParsedDataStorageProxy::removeProject(storage, project);
    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 0);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_addAndRemoveMultipleFilesFromProject()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    ParsedDataStorageProxy::addProject(storage, firstProject);
    ParsedDataStorageProxy::addProject(storage, secondProject);
    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 2);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 0);

    const QString path = pathFromName("testFile");
    addFileToProject(storage, firstProject, "content", path);
    addFileToProject(storage, secondProject, "content", path);

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 2);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 1);

    auto projects = storage->getProjectsForFile(path);
    QCOMPARE(projects.size(), 2);

    auto files = storage->getFilesFromProject(firstProject);
    QCOMPARE(files.size(), 1);

    files = storage->getFilesFromProject(secondProject);
    QCOMPARE(files.size(), 1);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_removeNonemptyProject()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QString project("testProject");
    ParsedDataStorageProxy::addProject(storage, project);

    addFileToProject(storage, project, "content", pathFromName("testFile"));

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 1);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 1);

    ParsedDataStorageProxy::removeProject(storage, project);

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 0);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 0);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_updateFileInOneProject()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QString project("testProject");
    ParsedDataStorageProxy::addProject(storage, project);

    const QString fileName("testFile");
    addFileToProject(storage, project, "content", pathFromName(fileName));

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 1);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 1);

    const QString refreshedContent = "changed content";
    addFileToProject(storage, project, refreshedContent, pathFromName(fileName));

    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 1);

    const QList<std::shared_ptr<ParsedDocument>> files = storage->getFilesFromProject(project);

    QCOMPARE(files.size(), 1);
    const DocumentSource received = files.at(0)->source();

    QCOMPARE(received.contents(), refreshedContent);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_updateFileInMultipleProjects()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    ParsedDataStorageProxy::addProject(storage, firstProject);
    ParsedDataStorageProxy::addProject(storage, secondProject);

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 2);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 0);

    const QString path = pathFromName("testFile");
    addFileToProject(storage, firstProject, "content", path);
    addFileToProject(storage, secondProject, "content", path);

    const QString refreshedContent = "changed content";
    addFileToProject(storage, firstProject, refreshedContent, path);

    QList<std::shared_ptr<ParsedDocument>> files = storage->getFilesFromProject(firstProject);
    QCOMPARE(files.size(), 1);

    const DocumentSource received = files.at(0)->source();
    QCOMPARE(received.contents(), refreshedContent);

    files = storage->getFilesFromProject(secondProject);
    QCOMPARE(files.size(), 1);
    const DocumentSource receivedSecond = files.at(0)->source();

    QCOMPARE(receivedSecond.contents(), refreshedContent);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_getFilesFromNonExistingProject()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QList<std::shared_ptr<ParsedDocument>> files = storage->getFilesFromProject("");
    QVERIFY(files.empty());

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::addFileToProject(ParsedDataStorage *storage,
                                              const QString &project,
                                              const QString &fileContent,
                                              const QString &filePath)
{
    const DocumentSource info(filePath, fileContent);
    std::unique_ptr<ParsedDocument> parsedDocument(new ParsedDocument(std::make_unique<Data::Module>(), info));
    ParsedDataStorageProxy::addFileToProject(storage, project, std::move(parsedDocument));
}

QString ParsedDataStorageTests::pathFromName(const QString &name)
{
    return QString("/test/file/path/") + name;
}

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

    addFileToProject(storage, project, path);
    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 1);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 1);

    auto documentFromProject = storage->getFileForPathFromProject(project, path);
    QVERIFY(documentFromProject != nullptr);

    auto documentAny = storage->getAnyFileForPath(path);
    QVERIFY(documentAny != nullptr);

    QVERIFY(documentFromProject == documentAny);

    auto projects = storage->getProjectsForFile(path);
    QVERIFY(projects.contains(project));

    auto files = storage->getFilesPathsFromProject(project);
    QCOMPARE(static_cast<int>(files.size()), 1);

    ParsedDataStorageProxy::removeFileFromProject(storage, project, path);

    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 0);

    documentFromProject = storage->getFileForPathFromProject(project, path);
    QVERIFY(documentFromProject == nullptr);

    projects = storage->getProjectsForFile(path);
    QVERIFY(projects.empty());

    files = storage->getFilesPathsFromProject(project);
    QCOMPARE(static_cast<int>(files.size()), 0);

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
    addFileToProject(storage, firstProject, path);
    addFileToProject(storage, secondProject, path);

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 2);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 2);

    auto projects = storage->getProjectsForFile(path);
    QCOMPARE(projects.size(), 2);

    auto files = storage->getFilesPathsFromProject(firstProject);
    QCOMPARE(static_cast<int>(files.size()), 1);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_removeNonemptyProject()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const QString project("testProject");
    ParsedDataStorageProxy::addProject(storage, project);

    addFileToProject(storage, project, pathFromName("testFile"));

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
    addFileToProject(storage, project, pathFromName(fileName));

    QCOMPARE(ParsedDataStorageProxy::getProjectsCount(storage), 1);
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 1);

    addFileToProject(storage, project, pathFromName(fileName));
    QCOMPARE(ParsedDataStorageProxy::getDocumentsCount(storage), 1);

    const auto files = storage->getFilesPathsFromProject(project);
    QCOMPARE(static_cast<int>(files.size()), 1);

    const QString path = files.at(0);
    const auto documentAny = storage->getAnyFileForPath(path);
    const auto documentFromProject = storage->getFileForPathFromProject(project, path);

    QVERIFY(documentAny == documentFromProject);

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
    addFileToProject(storage, firstProject, path);
    addFileToProject(storage, secondProject, path);

    auto files = storage->getFilesPathsFromProject(firstProject);
    QCOMPARE(static_cast<int>(files.size()), 1);

    files = storage->getFilesPathsFromProject(secondProject);
    QCOMPARE(static_cast<int>(files.size()), 1);

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::test_getFilesFromNonExistingProject()
{
    ParsedDataStorage *storage = ParsedDataStorageProxy::create();

    const auto files = storage->getFilesPathsFromProject("");
    QVERIFY(files.empty());

    ParsedDataStorageProxy::finish(storage);
}

void ParsedDataStorageTests::addFileToProject(ParsedDataStorage *storage,
                                              const QString &project,
                                              const QString &filePath)
{
    auto document = std::make_unique<Data::File>(filePath);
    ParsedDataStorageProxy::addFileToProject(storage, project, std::move(document));
}

QString ParsedDataStorageTests::pathFromName(const QString &name)
{
    return QString("/test/file/path/") + name;
}

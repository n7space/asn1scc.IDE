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

#include "parseddatastorage_tests.h"

#include <memory>

#include <QtTest>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

ParsedDataStorageTests::ParsedDataStorageTests(QObject *parent)
    : QObject(parent)
{}

void ParsedDataStorageTests::test_addAndRemoveProject()
{
    auto storage = new ParsedDataStorage;

    const QString project("testProject");

    storage->addProject(project);
    QCOMPARE(storage->getProjectsCount(), 1);
    QCOMPARE(storage->getDocumentsCount(), 0);

    storage->removeProject(project);
    QCOMPARE(storage->getProjectsCount(), 0);

    delete storage;
}

void ParsedDataStorageTests::test_addAndRemoveMultipleProjects()
{
    auto storage = new ParsedDataStorage;

    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    storage->addProject(firstProject);
    storage->addProject(secondProject);

    QCOMPARE(storage->getProjectsCount(), 2);
    QCOMPARE(storage->getDocumentsCount(), 0);

    storage->removeProject(firstProject);
    storage->removeProject(secondProject);

    QCOMPARE(storage->getProjectsCount(), 0);

    delete storage;
}

void ParsedDataStorageTests::test_addAndRemoveFileFromProject()
{
    auto storage = new ParsedDataStorage;

    const QString project("testProject");
    storage->addProject(project);

    const auto path = pathFromName("testFile");

    addFileToProject(storage, project, path);
    QCOMPARE(storage->getProjectsCount(), 1);
    QCOMPARE(storage->getDocumentsCount(), 1);

    auto documentFromProject = storage->getFileForPathFromProject(project, path);
    QVERIFY(documentFromProject != nullptr);

    auto documentAny = storage->getAnyFileForPath(path);
    QVERIFY(documentAny != nullptr);

    QVERIFY(documentFromProject == documentAny);

    auto projects = storage->getProjectsForFile(path);
    QVERIFY(projects.contains(project));

    auto files = storage->getFilesPathsFromProject(project);
    QCOMPARE(static_cast<int>(files.size()), 1);

    storage->removeFileFromProject(project, path);

    QCOMPARE(storage->getDocumentsCount(), 0);

    documentFromProject = storage->getFileForPathFromProject(project, path);
    QVERIFY(documentFromProject == nullptr);

    projects = storage->getProjectsForFile(path);
    QVERIFY(projects.empty());

    files = storage->getFilesPathsFromProject(project);
    QCOMPARE(static_cast<int>(files.size()), 0);

    storage->removeProject(project);
    QCOMPARE(storage->getProjectsCount(), 0);

    delete storage;
}

void ParsedDataStorageTests::test_addAndRemoveMultipleFilesFromProject()
{
    auto storage = new ParsedDataStorage;

    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    storage->addProject(firstProject);
    storage->addProject(secondProject);
    QCOMPARE(storage->getProjectsCount(), 2);
    QCOMPARE(storage->getDocumentsCount(), 0);

    const auto path = pathFromName("testFile");
    addFileToProject(storage, firstProject, path);
    addFileToProject(storage, secondProject, path);

    QCOMPARE(storage->getProjectsCount(), 2);
    QCOMPARE(storage->getDocumentsCount(), 2);

    auto projects = storage->getProjectsForFile(path);
    QCOMPARE(projects.size(), 2);

    auto files = storage->getFilesPathsFromProject(firstProject);
    QCOMPARE(static_cast<int>(files.size()), 1);

    delete storage;
}

void ParsedDataStorageTests::test_removeNonemptyProject()
{
    auto storage = new ParsedDataStorage;

    const QString project("testProject");
    storage->addProject(project);

    addFileToProject(storage, project, pathFromName("testFile"));

    QCOMPARE(storage->getProjectsCount(), 1);
    QCOMPARE(storage->getDocumentsCount(), 1);

    storage->removeProject(project);

    QCOMPARE(storage->getProjectsCount(), 0);
    QCOMPARE(storage->getDocumentsCount(), 0);

    delete storage;
}

void ParsedDataStorageTests::test_updateFileInOneProject()
{
    auto storage = new ParsedDataStorage;

    const QString project("testProject");
    storage->addProject(project);

    const QString fileName("testFile");
    addFileToProject(storage, project, pathFromName(fileName));

    QCOMPARE(storage->getProjectsCount(), 1);
    QCOMPARE(storage->getDocumentsCount(), 1);

    addFileToProject(storage, project, pathFromName(fileName));
    QCOMPARE(storage->getDocumentsCount(), 1);

    const auto files = storage->getFilesPathsFromProject(project);
    QCOMPARE(static_cast<int>(files.size()), 1);

    const auto path = files.at(0);
    const auto documentAny = storage->getAnyFileForPath(path);
    const auto documentFromProject = storage->getFileForPathFromProject(project, path);

    QVERIFY(documentAny == documentFromProject);

    delete storage;
}

void ParsedDataStorageTests::test_updateFileInMultipleProjects()
{
    auto storage = new ParsedDataStorage;

    const QString firstProject("testProject1");
    const QString secondProject("testProject2");

    storage->addProject(firstProject);
    storage->addProject(secondProject);

    QCOMPARE(storage->getProjectsCount(), 2);
    QCOMPARE(storage->getDocumentsCount(), 0);

    const auto path = pathFromName("testFile");
    addFileToProject(storage, firstProject, path);
    addFileToProject(storage, secondProject, path);

    auto files = storage->getFilesPathsFromProject(firstProject);
    QCOMPARE(static_cast<int>(files.size()), 1);

    files = storage->getFilesPathsFromProject(secondProject);
    QCOMPARE(static_cast<int>(files.size()), 1);

    delete storage;
}

void ParsedDataStorageTests::test_getFilesFromNonExistingProject()
{
    auto storage = new ParsedDataStorage;

    const auto files = storage->getFilesPathsFromProject("");
    QVERIFY(files.empty());

    delete storage;
}

void ParsedDataStorageTests::addFileToProject(ParsedDataStorage *storage,
                                              const QString &project,
                                              const Utils::FileName &filePath)
{
    auto document = std::make_unique<Data::File>(filePath.toString());
    storage->addFileToProject(project, std::move(document));
}

Utils::FileName ParsedDataStorageTests::pathFromName(const QString &name)
{
    return Utils::FileName::fromString("/test/file/path/" + name);
}

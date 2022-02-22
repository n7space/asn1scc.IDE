/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
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

#include "projectcontenthandler_tests.h"

#include <QSignalSpy>
#include <QtTest>

#include <projectcontenthandler.h>

#include "documentprocessorstub.h"
#include "sourcereadermock.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

ProjectContentHandlerTests::ProjectContentHandlerTests(QObject *parent)
    : QObject(parent)
    , m_storage(new ParsedDataStorage)
    , m_guard(new ModelValidityGuard)
    , m_createProcessor(
          [](const QString &) -> DocumentProcessor * { return new DocumentProcessorStub(); })
{
    m_fileTypes << "_ERROR_"
                << "_FAILED_"
                << "_SUCCESS_";
}

ProjectContentHandlerTests::~ProjectContentHandlerTests()
{
    delete m_storage;
    delete m_guard;
}

void ProjectContentHandlerTests::test_singleProjectAddedAndRemoved()
{
    QCOMPARE(m_guard->isValid(), true);

    QString projectName("TestProjectName_1");

    addProject(projectName);
    removeProject(projectName);
}

void ProjectContentHandlerTests::test_multipleProjectsAddedAndRemoved()
{
    QCOMPARE(m_guard->isValid(), true);

    QString firstProjectName("TestProjectName_1");
    QString secondProjectName("TestProjectName_2");

    addProject(firstProjectName);
    addProject(secondProjectName);

    removeProject(secondProjectName);
    removeProject(firstProjectName);
}

void ProjectContentHandlerTests::test_projectAddedAndRemovedTwice()
{
    QCOMPARE(m_guard->isValid(), true);

    QString projectName("TestProjectName_1");

    addProject(projectName);
    addProject(projectName);

    removeProject(projectName);
    removeProject(projectName);
}

void ProjectContentHandlerTests::test_singleFileAddedAndRemoved()
{
    QCOMPARE(m_guard->isValid(), true);
    QString projectName("TestProjectName_1");
    addProject(projectName);

    foreach (const QString type, m_fileTypes) {
        QStringList files("TestFileName" + type);
        fileListChanged(projectName, files);
        fileListChanged(projectName, QStringList());
    }

    fileListChanged(projectName, QStringList());
    removeProject(projectName);
}

void ProjectContentHandlerTests::test_multipleFilesAddedAndRemoved()
{
    QCOMPARE(m_guard->isValid(), true);

    QString projectName("TestProjectName_1");
    addProject(projectName);

    foreach (const QString type, m_fileTypes) {
        QStringList files;
        files.append("TestFileName1" + type);
        files.append("TestFileName2" + type);

        fileListChanged(projectName, files);
        fileListChanged(projectName, QStringList());
    }

    removeProject(projectName);
}

void ProjectContentHandlerTests::test_fileAddedToMultipleProjects()
{
    QCOMPARE(m_guard->isValid(), true);

    QString firstProjectName("TestProjectName_1");
    QString secondProjectName("TestProjectName_2");

    addProject(firstProjectName);
    addProject(secondProjectName);

    foreach (const QString type, m_fileTypes) {
        QStringList files("TestFileName_1" + type);

        fileListChanged(firstProjectName, files);
        fileListChanged(secondProjectName, files);
    }

    removeProject(firstProjectName);
    removeProject(secondProjectName);
}

void ProjectContentHandlerTests::test_fileContentChangedNoProject()
{
    QCOMPARE(m_guard->isValid(), true);

    foreach (const QString type, m_fileTypes) {
        QString file("TestFileName_1" + type);
        fileContentChanged(file);
    }
}

void ProjectContentHandlerTests::test_fileContentChanged()
{
    QCOMPARE(m_guard->isValid(), true);

    QString projectName("TestProjectName_1");
    addProject(projectName);

    foreach (const QString type, m_fileTypes) {
        QString file("TestFileName_1" + type);
        fileListChanged(projectName, QStringList(file));

        fileContentChanged(file);
        fileListChanged(projectName, QStringList());
    }

    removeProject(projectName);
}

void ProjectContentHandlerTests::test_fileInMultipleProjectContentChanged()
{
    QCOMPARE(m_guard->isValid(), true);

    QString firstProjectName("TestProjectName_1");
    QString secondProjectName("TestProjectName_2");

    addProject(firstProjectName);
    addProject(secondProjectName);

    foreach (const QString type, m_fileTypes) {
        QString file("TestFileName_1" + type);

        fileListChanged(firstProjectName, QStringList(file));
        fileListChanged(secondProjectName, QStringList(file));

        fileContentChanged(file);

        fileListChanged(firstProjectName, QStringList());
        fileListChanged(secondProjectName, QStringList());
    }

    removeProject(firstProjectName);
    removeProject(secondProjectName);
}

void ProjectContentHandlerTests::addProject(const QString &projectName)
{
    QSignalSpy spyAboutToUpdate(m_guard, &ModelValidityGuard::modelAboutToChange);
    QSignalSpy spyUpdated(m_guard, &ModelValidityGuard::modelChanged);

    ProjectContentHandler *pch = new ProjectContentHandler(m_createProcessor,
                                                           new SourceReaderMock,
                                                           m_storage,
                                                           m_guard);
    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_guard->isValid(), false);

    pch->handleProjectAdded(projectName);

    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_guard->isValid(), true);
}

void ProjectContentHandlerTests::removeProject(const QString &projectName)
{
    QSignalSpy spyAboutToUpdate(m_guard, &ModelValidityGuard::modelAboutToChange);
    QSignalSpy spyUpdated(m_guard, &ModelValidityGuard::modelChanged);

    ProjectContentHandler *pch = new ProjectContentHandler(m_createProcessor,
                                                           new SourceReaderMock,
                                                           m_storage,
                                                           m_guard);
    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_guard->isValid(), false);

    pch->handleProjectRemoved(projectName);

    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_guard->isValid(), true);
}

void ProjectContentHandlerTests::fileListChanged(const QString &projectName,
                                                 const QStringList &files)
{
    QSignalSpy spyAboutToUpdate(m_guard, &ModelValidityGuard::modelAboutToChange);
    QSignalSpy spyUpdated(m_guard, &ModelValidityGuard::modelChanged);

    ProjectContentHandler *pch = new ProjectContentHandler(m_createProcessor,
                                                           new SourceReaderMock,
                                                           m_storage,
                                                           m_guard);

    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_guard->isValid(), false);

    Utils::FilePaths nameList;
    for (const auto &file : files)
        nameList.append(Utils::FilePath::fromString(file));
    pch->handleFileListChanged(projectName, nameList);

    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_guard->isValid(), true);
}

void ProjectContentHandlerTests::fileContentChanged(const QString &path)
{
    QSignalSpy spyAboutToUpdate(m_guard, &ModelValidityGuard::modelAboutToChange);
    QSignalSpy spyUpdated(m_guard, &ModelValidityGuard::modelChanged);

    ProjectContentHandler *pch = new ProjectContentHandler(m_createProcessor,
                                                           new SourceReaderMock,
                                                           m_storage,
                                                           m_guard);

    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_guard->isValid(), false);

    pch->handleFileContentChanged(Utils::FilePath::fromString(path));

    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_guard->isValid(), true);
}

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

#include "projectcontenthandler_tests.h"

#include <QTest>

#include <QtTest>

#include <QSignalSpy>

#include "../documentprocessorfactory.h"
#include "../internalsourcereader.h"

#include "../projectcontenthandler.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

ProjectContentHandlerTests::ProjectContentHandlerTests(QObject *parent)
    : QObject(parent)
{
    m_tree = ModelTree::instance();
    m_storage = ParsedDataStorage::instance();

    m_fileTypes << "_ERROR_" << "_FAILED_" << "_SUCCESS_";
}

void ProjectContentHandlerTests::test_singleProjectAddedAndRemoved()
{
    QCOMPARE(m_tree->isValid(), true);

    QString projectName("TestProjectName_1");

    addProject(projectName);
    removeProject(projectName);
}

void ProjectContentHandlerTests::test_multipleProjectsAddedAndRemoved()
{
    QCOMPARE(m_tree->isValid(), true);

    QString firstProjectName("TestProjectName_1");
    QString secondProjectName("TestProjectName_2");

    addProject(firstProjectName);
    addProject(secondProjectName);

    removeProject(secondProjectName);
    removeProject(firstProjectName);
}

void ProjectContentHandlerTests::test_projectAddedAndRemovedTwice()
{
    QCOMPARE(m_tree->isValid(), true);

    QString projectName("TestProjectName_1");

    addProject(projectName);
    addProject(projectName);

    removeProject(projectName);
    removeProject(projectName);
}

void ProjectContentHandlerTests::test_singleFileAddedAndRemoved()
{
    QCOMPARE(m_tree->isValid(), true);
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
    QCOMPARE(m_tree->isValid(), true);

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
    QCOMPARE(m_tree->isValid(), true);

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
    QCOMPARE(m_tree->isValid(), true);

    foreach (const QString type, m_fileTypes) {
        QString file("TestFileName_1" + type);
        QString content("New Content" + type);
        fileContentChanged(file, content);
    }
}

void ProjectContentHandlerTests::test_fileContentChanged()
{
    QCOMPARE(m_tree->isValid(), true);

    QString projectName("TestProjectName_1");
    addProject(projectName);

    foreach (const QString type, m_fileTypes) {
        QString file("TestFileName_1" + type);
        fileListChanged(projectName, QStringList(file));

        QString content("New Content" + type);
        fileContentChanged(file, content);

        fileListChanged(projectName, QStringList());
    }

    removeProject(projectName);
}

void ProjectContentHandlerTests::test_fileInMultipleProjectContentChanged()
{
    QCOMPARE(m_tree->isValid(), true);

    QString firstProjectName("TestProjectName_1");
    QString secondProjectName("TestProjectName_2");

    addProject(firstProjectName);
    addProject(secondProjectName);

    foreach (const QString type, m_fileTypes) {
        QString file("TestFileName_1" + type);

        fileListChanged(firstProjectName, QStringList(file));
        fileListChanged(secondProjectName, QStringList(file));

        QString content("New Content" + type);
        fileContentChanged(file, content);

        fileListChanged(firstProjectName, QStringList());
        fileListChanged(secondProjectName, QStringList());
    }

    removeProject(firstProjectName);
    removeProject(secondProjectName);
}

void ProjectContentHandlerTests::addProject(const QString &projectName)
{
    QSignalSpy spyAboutToUpdate(m_tree, &ModelTree::modelAboutToUpdate);
    QSignalSpy spyUpdated(m_tree, &ModelTree::modelUpdated);

    ProjectContentHandler *pch = new ProjectContentHandler(DocumentProcessorFactory(DocumentProcessorFactory::WorkMode::Test),
                                                           new InternalSourceReader,
                                                           m_tree,
                                                           m_storage);
    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_tree->isValid(), false);

    pch->handleProjectAdded(projectName);

    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_tree->isValid(), true);
}

void ProjectContentHandlerTests::removeProject(const QString &projectName)
{
    QSignalSpy spyAboutToUpdate(m_tree, &ModelTree::modelAboutToUpdate);
    QSignalSpy spyUpdated(m_tree, &ModelTree::modelUpdated);

    ProjectContentHandler *pch = new ProjectContentHandler(DocumentProcessorFactory(DocumentProcessorFactory::WorkMode::Test),
                                                           new InternalSourceReader,
                                                           m_tree,
                                                           m_storage);
    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_tree->isValid(), false);

    pch->handleProjectRemoved(projectName);

    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_tree->isValid(), true);
}

void ProjectContentHandlerTests::fileListChanged(const QString &projectName, const QStringList &files)
{
    QSignalSpy spyAboutToUpdate(m_tree, &ModelTree::modelAboutToUpdate);
    QSignalSpy spyUpdated(m_tree, &ModelTree::modelUpdated);

    ProjectContentHandler *pch = new ProjectContentHandler(DocumentProcessorFactory(DocumentProcessorFactory::WorkMode::Test),
                                                           new InternalSourceReader,
                                                           m_tree,
                                                           m_storage);

    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_tree->isValid(), false);

    pch->handleFileListChanged(projectName, files);

    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_tree->isValid(), true);
}

void ProjectContentHandlerTests::fileContentChanged(const QString &path, const QString &content)
{
    QSignalSpy spyAboutToUpdate(m_tree, &ModelTree::modelAboutToUpdate);
    QSignalSpy spyUpdated(m_tree, &ModelTree::modelUpdated);

    ProjectContentHandler *pch = new ProjectContentHandler(DocumentProcessorFactory(DocumentProcessorFactory::WorkMode::Test),
                                                           new InternalSourceReader,
                                                           m_tree,
                                                           m_storage);

    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_tree->isValid(), false);

    pch->handleFileContentChanged(path, content, 1);

    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_tree->isValid(), true);
}

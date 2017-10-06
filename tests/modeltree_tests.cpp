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

#include "modeltree_tests.h"

#include <QtTest>
#include <QSignalSpy>

#include "modeltreenode.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

ModelTreeTests::ModelTreeTests(QObject *parent)
    : QObject(parent)
{
}

void ModelTreeTests::test_addAndRemoveProject()
{
    ModelTree *tree = ModelTreeProxy::create();

    const QString project("ProjectName");
    auto projectNode = ModelTreeNode::makePtr(project);

    ModelTreeProxy::addProjectNode(tree, projectNode);
    auto rootNode = tree->getModelTreeRoot();

    QVERIFY(rootNode != nullptr);
    QVERIFY(rootNode->getChildByName(project) != nullptr);

    ModelTreeProxy::removeProjectNode(tree, project);
    QVERIFY(rootNode->getChildByName(project) == nullptr);

    ModelTreeProxy::finish(tree);
}

void ModelTreeTests::test_getNonexistingNode()
{
    ModelTree *tree = ModelTreeProxy::create();

    ModelTreeNode::ModelTreeNodePtr node = tree->getAnyNodeForFilepath("filePath");
    QVERIFY(node == nullptr);

    ModelTreeProxy::finish(tree);
}

void ModelTreeTests::test_getNodeFromNonexistingProject()
{
    ModelTree *tree = ModelTreeProxy::create();

    ModelTreeNode::ModelTreeNodePtr node = tree->getNodeForFilepathFromProject("ProjectName", "filePath");
    QVERIFY(node == nullptr);

    ModelTreeProxy::finish(tree);
}

void ModelTreeTests::test_getFileListFromNonexisitngProject()
{
    ModelTree *tree = ModelTreeProxy::create();

    const QStringList list = tree->getFileListFromProject("ProjectName");
    QVERIFY(list.empty());

    ModelTreeProxy::finish(tree);
}

void ModelTreeTests::test_getFilesCntInNonexisitngProject()
{
    ModelTree *tree = ModelTreeProxy::create();

    const int cnt = tree->getProjectFilesCnt("ProjectName");
    QCOMPARE(cnt, 0);

    ModelTreeProxy::finish(tree);
}

void ModelTreeTests::test_modifiersCount()
{
    ModelTree *tree = ModelTreeProxy::create();

    QSignalSpy spyAboutToUpdate(tree, &ModelTree::modelAboutToUpdate);
    QSignalSpy spyUpdated(tree, &ModelTree::modelUpdated);

    const int modifiersCnt = 3;
    for (int i = 0; i < modifiersCnt; i++) {
        ModelTreeProxy::treeAboutToChange(tree);
        QVERIFY(!tree->isValid());
    }

    for (int i = 0; i < modifiersCnt; i++)
        ModelTreeProxy::treeChanged(tree);

    QVERIFY(tree->isValid());

    QCOMPARE(spyAboutToUpdate.count(), modifiersCnt);
    QCOMPARE(spyUpdated.count(), 1);

    ModelTreeProxy::finish(tree);
}

void ModelTreeTests::test_addAndRemoveNodesWithinProject()
{
    ModelTree *tree = ModelTreeProxy::create();

    const QString project("ProjectName");
    addProjectNode(tree, project);

    const int nodesCnt = 3;

    for (int i = 0; i < nodesCnt; i++)
        createNodeInProject(tree, project, QString::number(i));

    QCOMPARE(tree->getProjectFilesCnt(project), nodesCnt);

    const QStringList fileList(tree->getFileListFromProject(project));
    QCOMPARE(fileList.size(), nodesCnt);

    for (int i = 0; i < nodesCnt; i++) {
        QVERIFY(nodeExistInProject(tree, project, QString::number(i)));
        ModelTreeProxy::removeNodeFromProject(tree, project, QString::number(i));
        QVERIFY(!nodeExistInProject(tree, project, QString::number(i)));
    }

    QCOMPARE(tree->getProjectFilesCnt(project), 0);

    ModelTreeProxy::finish(tree);
}

void ModelTreeTests::test_updateNode()
{
    ModelTree *tree = ModelTreeProxy::create();

    const QString project("ProjectName");
    addProjectNode(tree, project);

    QString path(QString("/path/fileName"));
    createNodeInProject(tree, project, path);
    ModelTreeNode::ModelTreeNodePtr node = tree->getNodeForFilepathFromProject(project, path);

    const int nodesCnt = 3;
    for (int i = 0; i < nodesCnt; i++) {
        auto childNode = ModelTreeNode::makePtr(QString::number(i));
        node->addChild(childNode);
    }

    QCOMPARE(node->childrenCount(), nodesCnt);

    std::shared_ptr<Data::File> parsedDocument(new Data::File(Data::Source("TODO", "TODO")));
    ModelTreeProxy::updateModelTreeNode(tree, path, parsedDocument);

    QCOMPARE(node->childrenCount(), 0);

    ModelTreeProxy::finish(tree);
}

void ModelTreeTests::addProjectNode(ModelTree *tree, const QString &projectName)
{
    auto projectNode = ModelTreeNode::makePtr(projectName);
    ModelTreeProxy::addProjectNode(tree, projectNode);
}

void ModelTreeTests::createNodeInProject(ModelTree *tree, const QString &project, const QString &path)
{
    auto node = ModelTreeNode::makePtr(path);
    ModelTreeProxy::addNodeToProject(tree, project, node);
}

bool ModelTreeTests::nodeExistInProject(const ModelTree *tree, const QString &project, const QString &path)
{
    return (tree->getAnyNodeForFilepath(path) != nullptr) &&
            (tree->getNodeForFilepathFromProject(project, path) != nullptr);
}

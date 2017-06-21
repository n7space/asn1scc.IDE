/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

#include "modeltree.h"

#include "utils/qtcassert.h"

#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;

static ModelTree *m_instance;
static QMutex m_instanceMutex;

ModelTree *ModelTree::instance()
{
    if (m_instance)
        return m_instance;

    QMutexLocker locker(&m_instanceMutex);

    if (!m_instance)
        m_instance = new ModelTree;

    return m_instance;
}

ModelTree::ModelTree()
{
    m_treeRoot = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode);

    ParsedDataStorage *storage = ParsedDataStorage::instance();
    connect(storage, &ParsedDataStorage::fileUpdated,
            this, &ModelTree::updateModelTreeNode);
}

void ModelTree::addProjectNode(ModelTreeNode::ModelTreeNodePtr projectNode)
{
    {
        QMutexLocker locker(&m_dataMutex);

        m_treeRoot->addChild(projectNode);
    }

    emit treeUpdated();
}

void ModelTree::removeProjectNode(const QString &projectName)
{
    {
        QMutexLocker locker(&m_dataMutex);

        m_treeRoot->removeChildByName(projectName);
    }

    emit treeUpdated();
}

void ModelTree::addNodeToProject(const QString &projectName,
                                  ModelTreeNode::ModelTreeNodePtr node)
{
    {
        QMutexLocker locker(&m_dataMutex);

        ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
        QTC_ASSERT(projectNode != nullptr, return );

        projectNode->addChild(node);

        ParsedDataStorage *storage = ParsedDataStorage::instance();
        std::shared_ptr<ParsedDocument> document = storage->getFileForPath(node->id());
        if (document != nullptr)
            document->bindModelTreeNode(node);
    }

    emit treeUpdated();
}

ModelTreeNode::ModelTreeNodePtr ModelTree::getModelTreeRoot() const
{
    return m_treeRoot;
}

ModelTreeNode::ModelTreeNodePtr
ModelTree::getAnyNodeForFilepath(const QString &filePath) const
{
    QMutexLocker locker(&m_dataMutex);

    int projectCnt = m_treeRoot->childrenCount();
    for (int i = 0; i < projectCnt; i++) {
        ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->childAt(i);
        ModelTreeNode::ModelTreeNodePtr fileNode = projectNode->getChildByName(filePath);
        if (fileNode != nullptr)
            return fileNode;
    }

    return nullptr;
}

ModelTreeNode::ModelTreeNodePtr
ModelTree::getNodeForFilepathFromProject(const QString &projectName, const QString &filePath) const
{
    QMutexLocker locker(&m_dataMutex);

    ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
    if (projectNode == nullptr)
        return nullptr;

    return projectNode->getChildByName(filePath);
}

int ModelTree::getProjectFilesCnt(const QString &projectName) const
{
    QMutexLocker locker(&m_dataMutex);

    ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
    if (projectNode == nullptr)
        return 0;

    return projectNode->childrenCount();
}

void ModelTree::removeStaleNodesFromProject(const QString &projectName, const QStringList &currentPaths)
{
    {
        QMutexLocker locker(&m_dataMutex);

        ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
        if (projectNode == nullptr)
            return;

        QList<QString> stalePaths;
        for (int i = 0; i < projectNode->childrenCount(); i++)
            stalePaths.push_back(projectNode->childAt(i)->id());

        QSet<QString> pathsToRemove = stalePaths.toSet().subtract(currentPaths.toSet());

        foreach(const QString &stalePath, pathsToRemove)
            projectNode->removeChildByName(stalePath);
    }

    emit treeUpdated();
}

void ModelTree::updateModelTreeNode(const QString &filePath,
                                      std::shared_ptr<ParsedDocument> document)
{
    {
        QMutexLocker locker(&m_dataMutex);

        int projectCnt = m_treeRoot->childrenCount();
        for (int i = 0; i < projectCnt; i++) {
            ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->childAt(i);
            ModelTreeNode::ModelTreeNodePtr fileNode = projectNode->getChildByName(filePath);
            if (fileNode != nullptr) {
                fileNode->removeChildren();
                document->bindModelTreeNode(fileNode);
            }
        }
    }

    emit treeUpdated();
}

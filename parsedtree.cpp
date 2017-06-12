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

#include "parsedtree.h"

#include "utils/qtcassert.h"

#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;

static ParsedTree *m_instance;
static QMutex m_instanceMutex;

ParsedTree *ParsedTree::instance()
{
    if (m_instance)
        return m_instance;

    QMutexLocker locker(&m_instanceMutex);

    if (!m_instance)
        m_instance = new ParsedTree;

    return m_instance;
}

ParsedTree::ParsedTree()
{
    m_treeRoot = ParsedTreeNode::ParsedTreeNodePtr(new ParsedTreeNode);

    ParsedDataStorage *storage = ParsedDataStorage::instance();
    connect(storage, &ParsedDataStorage::fileUpdated,
            this, &ParsedTree::updateParsedTreeNode);
}

void ParsedTree::addProjectNode(ParsedTreeNode::ParsedTreeNodePtr projectNode)
{
    QMutexLocker locker(&m_dataMutex);

    m_treeRoot->addChild(projectNode);

    locker.unlock();

    emit treeUpdated();
}

void ParsedTree::removeProjectNode(const QString &projectName)
{
    QMutexLocker locker(&m_dataMutex);

    m_treeRoot->removeChildByName(projectName);

    locker.unlock();

    emit treeUpdated();
}

void
ParsedTree::addNodeToProject(const QString &projectName, ParsedTreeNode::ParsedTreeNodePtr node)
{
    QMutexLocker locker(&m_dataMutex);

    ParsedTreeNode::ParsedTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
    QTC_ASSERT(projectNode != nullptr, return );

    projectNode->addChild(node);

    ParsedDataStorage *storage = ParsedDataStorage::instance();
    std::shared_ptr<ParsedDocument> document = storage->getFileForPath(node->id());
    if (document != nullptr)
        document->bindParsedTreeNode(node);

    locker.unlock();

    emit treeUpdated();
}

void ParsedTree::removeNodeFromProject(const QString &projectName, const QString &fileName)
{
    QMutexLocker locker(&m_dataMutex);

    ParsedTreeNode::ParsedTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
    QTC_ASSERT(projectNode != nullptr, return );

    projectNode->removeChildByName(fileName);

    locker.unlock();

    emit treeUpdated();
}

ParsedTreeNode::ParsedTreeNodePtr ParsedTree::getParsedTreeRoot() const
{
    return m_treeRoot;
}

ParsedTreeNode::ParsedTreeNodePtr
ParsedTree::getNodeForFilepath(const QString &filePath) const
{
    QMutexLocker locker(&m_dataMutex);

    int projectCnt = m_treeRoot->childrenCount();
    for (int i = 0; i < projectCnt; i++) {
        ParsedTreeNode::ParsedTreeNodePtr projectNode = m_treeRoot->childAt(i);
        ParsedTreeNode::ParsedTreeNodePtr fileNode = projectNode->getChildByName(filePath);
        if (fileNode != nullptr)
            return fileNode;
    }

    return nullptr;
}

void ParsedTree::updateParsedTreeNode(const QString &filePath,
                                      std::shared_ptr<ParsedDocument> document)
{
    QMutexLocker locker(&m_dataMutex);

    int projectCnt = m_treeRoot->childrenCount();
    for (int i = 0; i < projectCnt; i++) {
        ParsedTreeNode::ParsedTreeNodePtr projectNode = m_treeRoot->childAt(i);
        ParsedTreeNode::ParsedTreeNodePtr fileNode = projectNode->getChildByName(filePath);
        if (fileNode != nullptr) {
            fileNode->removeChildren();
            document->bindParsedTreeNode(fileNode);
        }
    }

    locker.unlock();

    emit treeUpdated();
}

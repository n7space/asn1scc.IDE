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

#include "modeltree.h"

#include "utils/qtcassert.h"

#include "parseddocument.h"
#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;

ModelTree *ModelTree::instance()
{
    static ModelTree instance_;
    return &instance_;
}

ModelTree::ModelTree() :
    m_modifiersCnt(0)
{
    m_treeRoot = ModelTreeNode::makePtr();

    ParsedDataStorage *storage = ParsedDataStorage::instance();
    connect(storage, &ParsedDataStorage::fileUpdated,
            this, &ModelTree::updateModelTreeNode);
}

void ModelTree::addProjectNode(ModelTreeNode::ModelTreeNodePtr projectNode)
{
    QMutexLocker locker(&m_dataMutex);

    m_treeRoot->addChild(projectNode);
}

void ModelTree::removeProjectNode(const QString &projectName)
{
    QMutexLocker locker(&m_dataMutex);

    m_treeRoot->removeChildByName(projectName);
}

void ModelTree::addNodeToProject(const QString &projectName,
                                 ModelTreeNode::ModelTreeNodePtr node)
{
    QMutexLocker locker(&m_dataMutex);

    ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
    QTC_ASSERT(projectNode != nullptr, return );

    projectNode->addChild(node);

    const auto file = ParsedDataStorage::instance()->getFileForPathFromProject(projectName, node->name());
    if (file == nullptr)
        return;

    ParsedDocument::bindModelTreeNode(file, node);
}

void ModelTree::removeNodeFromProject(const QString &projectName, const QString &fileName)
{
    QMutexLocker locker(&m_dataMutex);

    ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
    QTC_ASSERT(projectNode != nullptr, return );

    projectNode->removeChildByName(fileName);
}

const ModelTreeNode::ModelTreeNodePtr ModelTree::getModelTreeRoot() const
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

QStringList ModelTree::getFileListFromProject(const QString &projectName) const
{
    QMutexLocker locker(&m_dataMutex);

    ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
    if (projectNode == nullptr)
        return QStringList();

    QStringList paths;
    for (int i = 0; i < projectNode->childrenCount(); i++)
        paths.append(projectNode->childAt(i)->name());

    return paths;
}

int ModelTree::getProjectFilesCnt(const QString &projectName) const
{
    QMutexLocker locker(&m_dataMutex);

    ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->getChildByName(projectName);
    if (projectNode == nullptr)
        return 0;

    return projectNode->childrenCount();
}

bool ModelTree::isValid() const
{
    return m_modifiersCnt == 0;
}

void ModelTree::updateModelTreeNode(const QString &filePath,
                                    const Data::File *document)
{
    QMutexLocker locker(&m_dataMutex);

    int projectCnt = m_treeRoot->childrenCount();
    for (int i = 0; i < projectCnt; i++) {
        ModelTreeNode::ModelTreeNodePtr projectNode = m_treeRoot->childAt(i);
        ModelTreeNode::ModelTreeNodePtr fileNode = projectNode->getChildByName(filePath);
        if (fileNode != nullptr) {
            fileNode->removeChildren();
            ParsedDocument::bindModelTreeNode(document, fileNode);
        }
    }
}

void ModelTree::treeAboutToChange()
{
    QMutexLocker locker(&m_dataMutex);

    m_modifiersCnt++;

    emit modelAboutToUpdate();
}

void ModelTree::treeChanged()
{
    {
        QMutexLocker locker(&m_dataMutex);

        m_modifiersCnt--;
        if (m_modifiersCnt != 0)
            return;
    }

    emit modelUpdated();
}

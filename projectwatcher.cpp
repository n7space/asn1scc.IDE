/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "projectwatcher.h"

#include <QRegularExpression>
#include <QTextDocument>
#include <QByteArray>
#include <QString>

#include <projectexplorer/session.h>

#include "documentprocessor.h"
#include "parsedtreenode.h"

using namespace Asn1Acn::Internal;

static const QRegularExpression DOCUMENT_FILTER_EXTENSIONS_REGEXP("\\.acn$|\\.asn1?$");

ProjectWatcher::ProjectWatcher()
{
    ProjectExplorer::SessionManager *sessionManager = ProjectExplorer::SessionManager::instance();
    connect(sessionManager, &ProjectExplorer::SessionManager::projectRemoved,
            this, &ProjectWatcher::onProjectRemoved);
    connect(sessionManager, &ProjectExplorer::SessionManager::projectAdded,
            this, &ProjectWatcher::onProjectAdded);

    ProjectExplorer::ProjectTree *projectParsedTree = ProjectExplorer::ProjectTree::instance();
    connect(projectParsedTree, &ProjectExplorer::ProjectTree::filesAboutToBeAdded,
            this, &ProjectWatcher::onFilesAdded);
    connect(projectParsedTree, &ProjectExplorer::ProjectTree::filesAboutToBeRemoved,
            this, &ProjectWatcher::onFilesRemoved);
    connect(projectParsedTree, &ProjectExplorer::ProjectTree::foldersAboutToBeRemoved,
            this, &ProjectWatcher::onFolderRemoved);

    m_storage = ParsedDataStorage::instance();
    m_tree = ParsedTree::instance();
}

void ProjectWatcher::onProjectAdded(ProjectExplorer::Project *project)
{
    QString projectName = project->displayName();

    auto node = ParsedTreeNode::ParsedTreeNodePtr(new ParsedTreeNode(projectName));
    m_tree->addProjectNode(node);
}

void ProjectWatcher::onProjectRemoved(ProjectExplorer::Project *project)
{
    m_tree->removeProjectNode(project->displayName());
}

void ProjectWatcher::onFilesAdded(ProjectExplorer::FolderNode *folder,
                                  const QList<ProjectExplorer::FileNode *> &newFiles)
{
    Q_UNUSED(folder);

    Utils::FileNameList validPaths = filterValidPaths(newFiles);
    ProjectExplorer::Project *project = ProjectExplorer::SessionManager::projectForNode(folder);

    foreach (const Utils::FileName &path, validPaths) {
        auto node = ParsedTreeNode::ParsedTreeNodePtr(new ParsedTreeNode(path.toString()));
        m_tree->addNodeToProject(project->displayName(), node);
    }

    processFiles(validPaths);
}

void ProjectWatcher::onFilesRemoved(ProjectExplorer::FolderNode *folder,
                                    const QList<ProjectExplorer::FileNode *> &staleFiles)
{
    Q_UNUSED(folder);

    Utils::FileNameList validPaths = filterValidPaths(staleFiles);
    ProjectExplorer::Project *project = ProjectExplorer::SessionManager::projectForNode(folder);

    foreach (const Utils::FileName &path, validPaths)
        m_tree->removeNodeFromProject(project->displayName(), path.toString());

    tryRemoveFiles(validPaths);
}

void ProjectWatcher::onFolderRemoved(ProjectExplorer::FolderNode *parentFolder,
                                     const QList<ProjectExplorer::FolderNode*> &staleFolders)
{
    Q_UNUSED(parentFolder);

    // If removed file is the last one in folder, than filesAboutToBeRemoved notification is not sent,
    // so replacement is produced here.

    foreach (ProjectExplorer::FolderNode* folder, staleFolders) {
        const QList<ProjectExplorer::FileNode *> staleFiles = folder->fileNodes();
        onFilesRemoved(folder, staleFiles);
    }
}

Utils::FileNameList ProjectWatcher::filterValidPaths(QList<ProjectExplorer::FileNode *> fileNodes)
{
    Utils::FileNameList result;

    // TODO: filtering should be performed using mimetypes
    QRegularExpression docFilter(DOCUMENT_FILTER_EXTENSIONS_REGEXP);

    foreach (const ProjectExplorer::FileNode *file, fileNodes) {
        QString displayPath = file->filePath().toString();
        if(docFilter.match(displayPath).hasMatch() == true)
            result.append(file->filePath());
    }

    return result;
}

void ProjectWatcher::processFiles(const Utils::FileNameList &filePaths) const
{
    foreach (const Utils::FileName &path, filePaths) {
        auto doc = textDocumentFromPath(path.toString());
        DocumentProcessor dp(doc.get(), path.toString(), -1);
        dp.run();
    }
}

void ProjectWatcher::tryRemoveFiles(const Utils::FileNameList &filePaths) const
{
    foreach (const Utils::FileName &path, filePaths) {
        QList<ProjectExplorer::Node *> nodes = ProjectExplorer::SessionManager::nodesForFile(path);
        // File should be removed from storage if it is present in one project only
        if (nodes.size() == 1)
            m_storage->removeFile(path.toString());
    }
}

std::unique_ptr<QTextDocument>
ProjectWatcher::textDocumentFromPath(const QString &fileName) const
{
    QFile file(fileName);

    auto textDocument = std::unique_ptr<QTextDocument>(new QTextDocument);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return nullptr;

    QString docContent = QString::fromLatin1(file.readAll().data());
    textDocument->setPlainText(docContent);

    return std::move(textDocument);
}

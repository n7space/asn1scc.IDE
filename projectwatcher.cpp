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

#include <utils/qtcassert.h>

#include "documentprocessor.h"
#include "modeltreenode.h"

using namespace Asn1Acn::Internal;

static const QRegularExpression DOCUMENT_FILTER_EXTENSIONS_REGEXP("\\.acn$|\\.asn1?$");

ProjectWatcher::ProjectWatcher()
{
    ProjectExplorer::SessionManager *sessionManager = ProjectExplorer::SessionManager::instance();
    connect(sessionManager, &ProjectExplorer::SessionManager::projectRemoved,
            this, &ProjectWatcher::onProjectRemoved);
    connect(sessionManager, &ProjectExplorer::SessionManager::projectAdded,
            this, &ProjectWatcher::onProjectAdded);

    m_storage = ParsedDataStorage::instance();
    m_tree = ModelTree::instance();
}

void ProjectWatcher::onProjectAdded(ProjectExplorer::Project *project)
{
    QString projectName = project->displayName();

    auto node = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(projectName));
    m_tree->addProjectNode(node);

    connect(project, &ProjectExplorer::Project::fileListChanged,
            this, &ProjectWatcher::onProjectFileListChanged);
}

void ProjectWatcher::onProjectRemoved(ProjectExplorer::Project *project)
{
    m_tree->removeProjectNode(project->displayName());

    disconnect(project, &ProjectExplorer::Project::fileListChanged,
               this, &ProjectWatcher::onProjectFileListChanged);
}

void ProjectWatcher::onProjectFileListChanged()
{
    ProjectExplorer::Project *project = qobject_cast<ProjectExplorer::Project *>(sender());
    QTC_ASSERT(project != nullptr, return);

    QString projectName = project->displayName();
    int storedFilesCnt = m_tree->getProjectFilesCnt(projectName);

    QStringList validFiles = filterValidPaths(project->files(ProjectExplorer::Project::AllFiles));
    int currentFilesCnt = validFiles.count();

    if (currentFilesCnt > storedFilesCnt)
        handleFilesAdded(projectName, validFiles);
    else if (currentFilesCnt < storedFilesCnt)
        handleFilesRemoved(projectName, validFiles);
}

QStringList ProjectWatcher::filterValidPaths(const QStringList &paths)
{
    // TODO: filtering should be performed using mimetypes
    return paths.filter(QRegularExpression(DOCUMENT_FILTER_EXTENSIONS_REGEXP));
}

void ProjectWatcher::handleFilesAdded(const QString &projectName, const QStringList &filePaths)
{
    foreach(const QString &path, filePaths) {
        if (m_tree->getNodeForFilepathFromProject(projectName, path) != nullptr)
            continue;

        auto node = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(path));
        m_tree->addNodeToProject(projectName, node);
    }

    tryProcessFiles(filePaths);
}

void ProjectWatcher::handleFilesRemoved(const QString &projectName, const QStringList &filePaths)
{
    m_tree->removeStaleNodesFromProject(projectName, filePaths);
    tryRemoveFiles(filePaths);
}

void ProjectWatcher::tryProcessFiles(const QStringList &filePaths) const
{
    foreach (const QString &path, filePaths) {
        if (m_storage->getFileForPath(path) != nullptr)
            continue;

        auto doc = textDocumentFromPath(path);
        DocumentProcessor *dp = new DocumentProcessor(doc.get(), path, -1);

        connect(dp, &DocumentProcessor::processingFinished,
                [dp](){dp->deleteLater();});

        dp->run();
    }
}

void ProjectWatcher::tryRemoveFiles(const QStringList &filePaths) const
{
    Q_UNUSED(filePaths);

    foreach(const QString &path, filePaths) {
        if (m_tree->getAnyNodeForFilepath(path))
            m_storage->removeFile(path);
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

    file.close();

    return std::move(textDocument);
}

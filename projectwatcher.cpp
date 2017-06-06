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

#include "asndocumentprocessor.h"
#include "asnparseddatastorage.h"

using namespace Asn1Acn::Internal;

static const QString DOCUMENT_FILTER_EXTENSIONS_REGEXP("\\.acn$|\\.asn1?$");

ProjectWatcher::ProjectWatcher()
{
    ProjectExplorer::SessionManager *sessionManager = ProjectExplorer::SessionManager::instance();
    connect(sessionManager, &ProjectExplorer::SessionManager::projectRemoved,
            this, &ProjectWatcher::onProjectRemoved);

    ProjectExplorer::ProjectTree *projectTree = ProjectExplorer::ProjectTree::instance();
    connect(projectTree, &ProjectExplorer::ProjectTree::filesAboutToBeAdded,
            this, &ProjectWatcher::onFilesAdded);

    connect(projectTree, &ProjectExplorer::ProjectTree::filesAboutToBeRemoved,
            this, &ProjectWatcher::onFilesRemoved);
}

void ProjectWatcher::onProjectRemoved(ProjectExplorer::Project *project)
{
    if (project == nullptr)
        return;

    QStringList validFilePaths = filterValidFiles(project->files(ProjectExplorer::Project::AllFiles));

    removeFiles(validFilePaths);
}

void ProjectWatcher::onFilesAdded(ProjectExplorer::FolderNode *folder,
                                  const QList<ProjectExplorer::FileNode *> &newFiles)
{
    Q_UNUSED(folder);

    QStringList filePaths = getFilePaths(newFiles);
    QStringList validFilePaths = filterValidFiles(filePaths);
    processFiles(validFilePaths);
}

void ProjectWatcher::onFilesRemoved(ProjectExplorer::FolderNode *folder,
                                    const QList<ProjectExplorer::FileNode *> &staleFiles)
{
    Q_UNUSED(folder);

    QStringList filePaths = getFilePaths(staleFiles);
    QStringList validFilePaths = filterValidFiles(filePaths);
    removeFiles(validFilePaths);
}

QStringList ProjectWatcher::getFilePaths(const QList<ProjectExplorer::FileNode *> &fileNodes) const
{
    QStringList filePaths;
    foreach (const ProjectExplorer::FileNode *file, fileNodes)
        filePaths << file->filePath().toString();

    return filePaths;
}

QStringList ProjectWatcher::filterValidFiles(const QStringList &allFiles) const
{
    // TODO: filtering should be performed using mimetypes?
    return allFiles.filter(QRegularExpression(DOCUMENT_FILTER_EXTENSIONS_REGEXP));
}

void ProjectWatcher::processFiles(const QStringList &filePaths) const
{
    foreach (const QString &path, filePaths) {
        auto doc = textDocumentFromPath(path);
        AsnDocumentProcessor dp(doc.get(), path, -1);
        dp.run();
    }
}

void ProjectWatcher::removeFiles(const QStringList &filePaths) const
{
    AsnParsedDataStorage *storage = AsnParsedDataStorage::instance();
    foreach (const QString &path, filePaths)
        storage->removeFile(path);
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

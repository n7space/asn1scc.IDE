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

#include "projectcontenthandler.h"

#include <QList>

#include "documentprocessor.h"

using namespace Asn1Acn::Internal;

ProjectContentHandler::ProjectContentHandler() :
    m_projectsChanged(0)
{
    m_tree = ModelTree::instance();
    m_storage = ParsedDataStorage::instance();
}

void ProjectContentHandler::handleProjectAdded(const QString &projectName)
{
    m_tree->treeAboutToChange();

    m_storage->addProject(projectName);
    auto node = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(projectName));
    m_tree->addProjectNode(node);

    allProcessingFinished();
}

void ProjectContentHandler::handleProjectRemoved(const QString &projectName)
{
    m_tree->treeAboutToChange();

    m_tree->removeProjectNode(projectName);
    m_storage->removeProject(projectName);

    allProcessingFinished();
}

void ProjectContentHandler::handleFileListChanged(const QString &projectName, const QStringList &fileList)
{
    int storedFilesCnt = m_tree->getProjectFilesCnt(projectName);

    int currentFilesCnt = fileList.count();

    m_tree->treeAboutToChange();
    if (currentFilesCnt > storedFilesCnt)
        handleFilesAdded(projectName, fileList);
    else if (currentFilesCnt < storedFilesCnt)
        handleFilesRemoved(projectName, fileList);
}

void ProjectContentHandler::handleFileContentChanged(const QString &path, const QString &content, int revision)
{
    m_tree->treeAboutToChange();

    QStringList projects = m_storage->getProjectsForFile(path);
    for (const QString projectName : projects) {
        DocumentProcessor *docProcessor = new DocumentProcessor(projectName);

        QList<std::shared_ptr<ParsedDocument> > files = m_storage->getFilesFromProject(projectName);
        foreach (const std::shared_ptr<ParsedDocument> &file, files) {
            if (file->source().getPath() == path) {
                docProcessor->addToRun(content, path, revision);
            } else {
                docProcessor->addToRun(file->source().getContent(),
                                       file->source().getPath(),
                                       file->source().getRevision());
            }
        }

        connect(docProcessor, &DocumentProcessor::processingFinished,
                [this, docProcessor](){docProcessor->deleteLater();});

        connect(docProcessor, &DocumentProcessor::processingFinished,
                this, &ProjectContentHandler::onFilesProcessingFinished);

        docProcessor->run();

        m_projectsChanged++;
    }
}

void ProjectContentHandler::handleFilesAdded(const QString &projectName, const QStringList &filePaths)
{
    foreach (const QString &path, filePaths) {
        if (m_tree->getNodeForFilepathFromProject(projectName, path) != nullptr)
            continue;

        auto node = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(path));
        m_tree->addNodeToProject(projectName, node);
    }

    processFiles(projectName, filePaths);
}

void ProjectContentHandler::handleFilesRemoved(const QString &projectName, const QStringList &filePaths)
{
    m_tree->removeStaleNodesFromProject(projectName, filePaths);
    m_storage->cleanProject(projectName);

    // FIXME: if all the files were removed from project, than asn1scc daemon will crash
    processFiles(projectName, filePaths);
}

void ProjectContentHandler::processFiles(const QString &projectName, const QStringList &filePaths)
{
    DocumentProcessor *dp = new DocumentProcessor(projectName);

    foreach (const QString &path, filePaths) {
        std::shared_ptr<ParsedDocument> parsedDoc = m_storage->getFileForPath(path);
        if (parsedDoc != nullptr)
            dp->addToRun(parsedDoc->source().getContent(), path, parsedDoc->source().getRevision());
        else
            dp->addToRun(readFileContent(path), path, -1);
    }

    connect(dp, &DocumentProcessor::processingFinished,
            [dp](){dp->deleteLater();});

    connect(dp, &DocumentProcessor::processingFinished,
            this, &ProjectContentHandler::onFilesProcessingFinished);

    dp->run();

    m_projectsChanged++;
}

QString
ProjectContentHandler::readFileContent(const QString &fileName) const
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QString docContent = QString::fromLatin1(file.readAll().data());

    file.close();

    return docContent;
}

void ProjectContentHandler::onFilesProcessingFinished()
{
    DocumentProcessor *dp = qobject_cast<DocumentProcessor *>(sender());
    dp->deleteLater();

    if (--m_projectsChanged == 0)
        allProcessingFinished();
}

void ProjectContentHandler::allProcessingFinished()
{
    m_tree->treeChanged();
    deleteLater();
}

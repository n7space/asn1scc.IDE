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

#include "projectcontenthandler.h"

#include <QList>

#include <utils/qtcassert.h>

#include "asn1sccdocumentprocessor.h"

using namespace Asn1Acn::Internal;

ProjectContentHandler *ProjectContentHandler::create()
{
    return new ProjectContentHandler(DocumentProcessorFactory());
}

ProjectContentHandler::ProjectContentHandler(const DocumentProcessorFactory &factory)
    : m_projectsChanged(0)
    , m_factory(factory)
{
    m_tree = ModelTree::instance();
    m_storage = ParsedDataStorage::instance();

    m_tree->treeAboutToChange();
}

ProjectContentHandler::~ProjectContentHandler()
{
    m_tree->treeChanged();
}

void ProjectContentHandler::handleProjectAdded(const QString &projectName)
{
    m_storage->addProject(projectName);
    auto node = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(projectName));
    m_tree->addProjectNode(node);

    allProcessingFinished();
}

void ProjectContentHandler::handleProjectRemoved(const QString &projectName)
{
    m_tree->removeProjectNode(projectName);
    m_storage->removeProject(projectName);

    allProcessingFinished();
}

void ProjectContentHandler::handleFileListChanged(const QString &projectName, const QStringList &fileList)
{
    removeStaleFiles(projectName, fileList);
    addNewFiles(projectName, fileList);
    processFiles(projectName, fileList);
}

void ProjectContentHandler::handleFileContentChanged(const QString &path, const QString &content, int revision)
{
    QStringList projects = m_storage->getProjectsForFile(path);

    if (projects.empty()) {
        allProcessingFinished();
        return;
    }

    for (const QString projectName : projects) {
        DocumentProcessor *docProcessor = createDocumentProcessorForFileChange(projectName, path, content, revision);
        startProcessing(docProcessor);
    }
}

void ProjectContentHandler::removeStaleFiles(const QString &projectName, const QStringList &filePaths)
{
    QStringList stalePaths = getStaleFilesNames(projectName, filePaths);

    foreach (const QString &stalePath, stalePaths) {
        m_tree->removeNodeFromProject(projectName, stalePath);
        m_storage->removeFileFromProject(projectName, stalePath);
    }
}

void ProjectContentHandler::addNewFiles(const QString &projectName, const QStringList &filePaths)
{
    foreach (const QString &path, filePaths) {
        if (m_tree->getNodeForFilepathFromProject(projectName, path) != nullptr)
            continue;

        auto node = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(path,
                                                                      Data::SourceLocation(path, 0, 0)));
        m_tree->addNodeToProject(projectName, node);
    }
}

void ProjectContentHandler::processFiles(const QString &projectName, const QStringList &filePaths)
{
    DocumentProcessor *dp = createDocumentProcessorForProjectChange(projectName, filePaths);
    startProcessing(dp);
}

QStringList ProjectContentHandler::getStaleFilesNames(const QString &projectName, const QStringList &filePaths) const
{
    QStringList staleFileList = m_tree->getFileListFromProject(projectName);
    QSet<QString> pathsToRemove = staleFileList.toSet().subtract(filePaths.toSet());

    QStringList ret;
    foreach (const QString &stalePath, pathsToRemove)
        ret.append(stalePath);

    return ret;
}

DocumentProcessor *ProjectContentHandler::createDocumentProcessorForFileChange(const QString &projectName,
                                                                               const QString &path,
                                                                               const QString &content,
                                                                               int revision) const
{
    DocumentProcessor *docProcessor = Asn1SccDocumentProcessor::create(projectName);

    QList<std::shared_ptr<ParsedDocument>> files = m_storage->getFilesFromProject(projectName);
    foreach (const std::shared_ptr<ParsedDocument> &file, files) {
        if (file->source().getPath() == path) {
            docProcessor->addToRun(content, path, revision);
        } else {
            docProcessor->addToRun(file->source().getContent(),
                                   file->source().getPath(),
                                   file->source().getRevision());
        }
    }

    return docProcessor;
}

DocumentProcessor *ProjectContentHandler::createDocumentProcessorForProjectChange(const QString &projectName,
                                                                                  const QStringList &filePaths) const
{
    DocumentProcessor *docProcessor = Asn1SccDocumentProcessor::create(projectName);

    foreach (const QString &path, filePaths) {
        std::shared_ptr<ParsedDocument> parsedDoc = m_storage->getFileForPath(path);
        if (parsedDoc != nullptr)
            docProcessor->addToRun(parsedDoc->source().getContent(), path, parsedDoc->source().getRevision());
        else
            docProcessor->addToRun(readFileContent(path), path, -1);
    }

    return docProcessor;
}

void ProjectContentHandler::startProcessing(DocumentProcessor *dp)
{
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

void ProjectContentHandler::onFilesProcessingFinished(const QString &projectName)
{
    DocumentProcessor *dp = qobject_cast<DocumentProcessor *>(sender());
    std::vector<std::unique_ptr<ParsedDocument>> parsedDocuments = dp->takeResults();

    switch (dp->getState()) {
        case DocumentProcessor::State::Successful:
            handleFilesProcesedWithSuccess(projectName, std::move(parsedDocuments));
            break;
        case DocumentProcessor::State::Errored:
        case DocumentProcessor::State::Failed:
            handleFilesProcesedWithFailure(projectName, std::move(parsedDocuments));
            break;
        default:
            break;
    }

    dp->deleteLater();

    if (--m_projectsChanged == 0)
        allProcessingFinished();
}

void ProjectContentHandler::handleFilesProcesedWithSuccess(const QString &projectName,
                                                          std::vector<std::unique_ptr<ParsedDocument>> parsedDocuments)
{
    for (size_t i = 0; i < parsedDocuments.size(); i++)
        m_storage->addFileToProject(projectName, std::move(parsedDocuments[i]));
}

void ProjectContentHandler::handleFilesProcesedWithFailure(const QString &projectName,
                                                          std::vector<std::unique_ptr<ParsedDocument>> parsedDocuments)
{
    for (size_t i = 0; i < parsedDocuments.size(); i++) {
        auto filePath = parsedDocuments[i]->source().getPath();
        if (m_storage->getFileForPath(filePath) != nullptr)
            continue;

        m_storage->addFileToProject(projectName, std::move(parsedDocuments[i]));
    }
}

void ProjectContentHandler::allProcessingFinished()
{
    deleteLater();
}

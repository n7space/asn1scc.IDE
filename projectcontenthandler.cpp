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
#include "filesourcereader.h"

using namespace Asn1Acn::Internal;

ProjectContentHandler *ProjectContentHandler::create()
{
    return new ProjectContentHandler([](const QString &project)->DocumentProcessor* { return Asn1SccDocumentProcessor::create(project); } ,
                                     new FileSourceReader,
                                     ModelTree::instance(),
                                     ParsedDataStorage::instance());
}

ProjectContentHandler::ProjectContentHandler(std::function<DocumentProcessor *(const QString &)> createProcessor,
                                             const SourceReader *sourceReader,
                                             ModelTree *tree,
                                             ParsedDataStorage *storage)
    : m_tree(tree)
    , m_storage(storage)
    , m_projectsChanged(0)
    , m_sourceReader(sourceReader)
    , m_createProcessor(createProcessor)
{
    ModelTreeProxy::treeAboutToChange(m_tree);
}

ProjectContentHandler::~ProjectContentHandler()
{
    delete m_sourceReader;
}

void ProjectContentHandler::handleProjectAdded(const QString &projectName)
{
    ParsedDataStorageProxy::addProject(m_storage, projectName);
    auto node = ModelTreeNode::makePtr(projectName);
    ModelTreeProxy::addProjectNode(m_tree, node);

    allProcessingFinished();
}

void ProjectContentHandler::handleProjectRemoved(const QString &projectName)
{
    ModelTreeProxy::removeProjectNode(m_tree, projectName);
    ParsedDataStorageProxy::removeProject(m_storage, projectName);

    allProcessingFinished();
}

void ProjectContentHandler::handleFileListChanged(const QString &projectName, const QStringList &fileList)
{
    removeStaleFiles(projectName, fileList);
    addNewFiles(projectName, fileList);
    processFiles(projectName, fileList);
}

void ProjectContentHandler::handleFileContentChanged(const QString &path, const QString &content)
{
    QStringList projects = m_storage->getProjectsForFile(path);

    if (projects.empty()) {
        allProcessingFinished();
        return;
    }

    for (const QString projectName : projects) {
        DocumentProcessor *docProcessor = createDocumentProcessorForFileChange(projectName, path, content);
        startProcessing(docProcessor);
    }
}

void ProjectContentHandler::removeStaleFiles(const QString &projectName, const QStringList &filePaths)
{
    QStringList stalePaths = getStaleFilesNames(projectName, filePaths);

    foreach (const QString &stalePath, stalePaths) {
        ModelTreeProxy::removeNodeFromProject(m_tree, projectName, stalePath);
        ParsedDataStorageProxy::removeFileFromProject(m_storage, projectName, stalePath);
    }
}

void ProjectContentHandler::addNewFiles(const QString &projectName, const QStringList &filePaths)
{
    foreach (const QString &path, filePaths) {
        if (m_tree->getNodeForFilepathFromProject(projectName, path) != nullptr)
            continue;

        auto node = ModelTreeNode::makePtr(path, Data::Type::UserDefined, Data::SourceLocation(path, 0, 0)); // TODO type?
        ModelTreeProxy::addNodeToProject(m_tree, projectName, node);
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
                                                                               const QString &content) const
{
    DocumentProcessor *docProcessor = m_createProcessor(projectName);

    QList<std::shared_ptr<ParsedDocument>> files = m_storage->getFilesFromProject(projectName);
    foreach (const std::shared_ptr<ParsedDocument> &file, files) {
        if (file->source().filePath() == path)
            docProcessor->addToRun(path, content);
        else
            docProcessor->addToRun(file->source().filePath(), file->source().contents());
    }

    return docProcessor;
}

DocumentProcessor *ProjectContentHandler::createDocumentProcessorForProjectChange(const QString &projectName,
                                                                                  const QStringList &filePaths) const
{
    DocumentProcessor *docProcessor = m_createProcessor(projectName);

    foreach (const QString &path, filePaths) {
        auto parsedDoc = m_storage->getFileForPath(path);
        if (parsedDoc != nullptr)
            docProcessor->addToRun(path, parsedDoc->source().contents());
        else
            docProcessor->addToRun(path, m_sourceReader->readContent(path));
    }

    return docProcessor;
}

void ProjectContentHandler::startProcessing(DocumentProcessor *dp)
{
    connect(dp, &DocumentProcessor::processingFinished,
            this, &ProjectContentHandler::onFilesProcessingFinished);

    m_projectsChanged++;

    dp->run();
}

void ProjectContentHandler::onFilesProcessingFinished(const QString &projectName)
{
    DocumentProcessor *dp = qobject_cast<DocumentProcessor *>(sender());

    switch (dp->getState()) {
        case DocumentProcessor::State::Successful:
            handleFilesProcesedWithSuccess(projectName, dp->takeResults());
            break;
        case DocumentProcessor::State::Errored:
        case DocumentProcessor::State::Failed:
            handleFilesProcesedWithFailure(projectName, dp->takeResults());
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
        ParsedDataStorageProxy::addFileToProject(m_storage, projectName, std::move(parsedDocuments[i]));
}

void ProjectContentHandler::handleFilesProcesedWithFailure(const QString &projectName,
                                                          std::vector<std::unique_ptr<ParsedDocument>> parsedDocuments)
{
    for (size_t i = 0; i < parsedDocuments.size(); i++) {
        auto filePath = parsedDocuments[i]->source().filePath();
        if (m_storage->getFileForPath(filePath) != nullptr)
            continue;

        ParsedDataStorageProxy::addFileToProject(m_storage, projectName, std::move(parsedDocuments[i]));
    }
}

void ProjectContentHandler::allProcessingFinished()
{
    ModelTreeProxy::treeChanged(m_tree);
    deleteLater();
}

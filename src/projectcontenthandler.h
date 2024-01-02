/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include <functional>

#include <QObject>
#include <QString>

#include <utils/fileutils.h>

#include "documentprocessor.h"
#include "modelvalidityguard.h"
#include "parseddatastorage.h"
#include "sourcereader.h"

namespace Asn1Acn {
namespace Internal {

class ProjectContentHandler : public QObject
{
    Q_OBJECT

public:
    static ProjectContentHandler *create();

    ProjectContentHandler(std::function<DocumentProcessor *(const QString &)>,
                          const SourceReader *sourceReader,
                          ParsedDataStorage *storage,
                          ModelValidityGuard *guard);

    ~ProjectContentHandler();

    void handleProjectAdded(const QString &projectName);
    void handleProjectRemoved(const QString &projectName);

    void handleFileListChanged(const QString &projectName, const Utils::FilePaths &fileList);
    void handleFileContentChanged(const Utils::FilePath &path);

private slots:
    void onFilesProcessingFinished(const QString &projectName);

private:
    void removeStaleFiles(const QString &projectName, const Utils::FilePaths &filePaths);

    Utils::FilePaths getStaleFilesPaths(const QString &projectName,
                                        const Utils::FilePaths &filePaths) const;

    void processFiles(const QString &projectName, const Utils::FilePaths &filePaths);

    DocumentProcessor *createDocumentProcessor(const QString &projectName,
                                               const Utils::FilePaths &filePaths) const;
    void startProcessing(DocumentProcessor *dp);
    void allProcessingFinished();

    void handleFilesProcesedWithSuccess(const QString &projectName,
                                        std::vector<std::unique_ptr<Data::File>> parsedDocuments);

    void handleFilesProcesedWithFailure(const QString &projectName,
                                        std::vector<std::unique_ptr<Data::File>> parsedDocuments,
                                        const std::vector<Data::ErrorMessage> &errorMessages);

    void refreshErrorMessages(Data::File *file,
                              const std::vector<Data::ErrorMessage> &errorMessages);

    ParsedDataStorage *m_storage;
    ModelValidityGuard *m_guard;

    unsigned m_projectsChanged;

    const SourceReader *m_sourceReader;

    std::function<DocumentProcessor *(const QString &)> m_createProcessor;
};

} // namespace Internal
} // namespace Asn1Acn

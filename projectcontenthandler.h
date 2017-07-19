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

#include <QObject>
#include <QString>
#include <QStringList>

#include "modeltree.h"
#include "parseddatastorage.h"
#include "documentprocessor.h"

namespace Asn1Acn {
namespace Internal {

class ProjectContentHandler : public QObject
{
    Q_OBJECT

public:
    ProjectContentHandler();
    ~ProjectContentHandler();

    void handleProjectAdded(const QString &projectName);
    void handleProjectRemoved(const QString &projectName);

    void handleFileListChanged(const QString &projectName, const QStringList &fileList);
    void handleFileContentChanged(const QString &path, const QString &content, int revision);

signals:
    void processingFinished();

private slots:
    void onFilesProcessingFinished(const QString &projectName);

private:
    void handleFilesAdded(const QString &projectName, const QStringList &filePaths);
    void handleFilesRemoved(const QString &projectName, const QStringList &filePaths);

    QStringList getStaleFilesNames(const QString &projectName, const QStringList &filePaths) const;

    void processFiles(const QString &projectName, const QStringList &filePaths);
    QString readFileContent(const QString &fileName) const;

    DocumentProcessor *createDocumentProcessorForFileChange(const QString &projectName,
                                                            const QString &path,
                                                            const QString &content,
                                                            int revision) const;
    DocumentProcessor *createDocumentProcessorForProjectChange(const QString &projectName,
                                                               const QStringList &filePaths) const;
    void startProcessing(DocumentProcessor *dp);
    void allProcessingFinished();

    ModelTree *m_tree;
    ParsedDataStorage *m_storage;

    unsigned m_projectsChanged;
};

} // namespace Internal
} // namespace Asn1Acn
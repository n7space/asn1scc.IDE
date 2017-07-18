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

#pragma once

#include <QHash>
#include <QMutex>
#include <QString>

#include <memory>

#include "parseddocument.h"
#include "modeltreenode.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDataStorage : public QObject
{
    Q_OBJECT

    ParsedDataStorage() = default;
    ~ParsedDataStorage() = default;

public:
    static ParsedDataStorage *instance();

    std::shared_ptr<ParsedDocument> getFileForPath(const QString &filePath) const;

    void addProject(const QString &projectName);
    void removeProject(const QString &projectName);

    void addFileToProject(const QString &projectName, std::unique_ptr<ParsedDocument> file);
    void removeFileFromProject(const QString &projectName, const QString &filePath);

    QStringList getProjectsForFile(const QString &filePath) const;
    QList<std::shared_ptr<ParsedDocument>> getFilesFromProject(const QString &projectName) const;

    void cleanProject(const QString &projectName);

signals:
    void fileUpdated(const QString &filePath, std::shared_ptr<ParsedDocument> newFile);

private:
    using Project = QHash<QString, std::shared_ptr<ParsedDocument>>;

    void refreshFileInProjects(std::shared_ptr<ParsedDocument> file, const QString &filePath);

    QStringList getProjectsForFileInternal(const QString &filePath) const;
    QList<std::shared_ptr<ParsedDocument>> getFilesFromProjectInternal(const QString &projectName) const;
    void removeFileFromProjectInternal(const QString &projectName, const QString &filePath);

    QHash<QString, std::shared_ptr<ParsedDocument>> m_documents;
    QHash<QString, Project> m_projects;
    mutable QMutex m_documentsMutex;
};

} // namespace Internal
} // namespace Asn1Acn

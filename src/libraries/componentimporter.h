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

#pragma once

#include <QDir>
#include <QString>

#include <utils/fileutils.h>

namespace ProjectExplorer {
class Project;
}

namespace Asn1Acn {
namespace Internal {
namespace Libraries {

class ComponentImporter
{
public:
    ComponentImporter(ProjectExplorer::Project *project);

    void setDirectory(const QString &path);
    void setFiles(const Utils::FilePaths &files);
    void setProject(ProjectExplorer::Project *project);

    const Utils::FilePaths &sourceFiles() const;
    const Utils::FilePaths &importedFiles() const;

    void import();

private:
    Utils::FilePaths copyFilesToProject();
    void createTargetDir(QDir &parent, const QString &path);
    QString targetFileName(const QString &file);
    void addFilesToProject(const Utils::FilePaths &files);
    void copyFile(const QString &source, const QString &target);
    Utils::FilePaths createUniqueFilesList(const ProjectExplorer::Project *project,
                                           const Utils::FilePaths &newFiles);

    Utils::FilePaths m_sourceFiles;
    Utils::FilePaths m_importedFiles;

    QDir m_sourceDir;
    QDir m_targetDir;

    ProjectExplorer::Project *m_project;
};

} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

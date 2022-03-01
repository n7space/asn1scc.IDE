/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
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

#include "componentimporter.h"

#include <stdexcept>

#include <QFile>

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/session.h>

using namespace Asn1Acn::Internal::Libraries;

ComponentImporter::ComponentImporter(ProjectExplorer::Project *project)
    : m_project(project)
{}

void ComponentImporter::setFiles(const Utils::FilePaths &files)
{
    m_sourceFiles = files;
}

void ComponentImporter::setProject(ProjectExplorer::Project *project)
{
    m_project = project;
}

const Utils::FilePaths &ComponentImporter::sourceFiles() const
{
    return m_sourceFiles;
}

const Utils::FilePaths &ComponentImporter::importedFiles() const
{
    return m_importedFiles;
}

void ComponentImporter::setDirectory(const QString &path)
{
    m_sourceDir = QDir(path);
}

void ComponentImporter::import()
{
    const auto projectDir = QDir(m_project->projectDirectory().toString());
    m_targetDir = QDir(projectDir.filePath(m_sourceDir.dirName()));

    const auto copied = copyFilesToProject();
    addFilesToProject(copied);
}

Utils::FilePaths ComponentImporter::copyFilesToProject()
{
    Utils::FilePaths copiedFiles;

    for (const auto &file : m_sourceFiles) {
        const QString filePath = file.path();
        const QString target = targetFileName(filePath);

        createTargetDir(m_targetDir, QFileInfo(target).absolutePath());
        copyFile(filePath, target);
        copiedFiles.append(Utils::FilePath::fromString(target));
    }

    return copiedFiles;
}

void ComponentImporter::createTargetDir(QDir &parent, const QString &path)
{
    if (!parent.mkpath(path))
        throw std::runtime_error("Could not create directory \'"
                                 + parent.filePath(path).toStdString() + "\'");
}

void ComponentImporter::copyFile(const QString &source, const QString &target)
{
    if (QFile::exists(target) && !QFile::remove(target))
        throw std::runtime_error("Could not replace exisitng file \'" + target.toStdString() + "\'");

    if (!QFile::copy(source, target))
        throw std::runtime_error("Could not copy \'" + source.toStdString() + "\' to \'"
                                 + target.toStdString() + "\'");
}

QString ComponentImporter::targetFileName(const QString &file)
{
    QString relativeSrcPath = m_sourceDir.relativeFilePath(file);
    return m_targetDir.filePath(relativeSrcPath);
}

void ComponentImporter::addFilesToProject(const Utils::FilePaths &files)
{
    m_importedFiles = createUniqueFilesList(m_project, files);
    m_project->rootProjectNode()->addFiles(m_importedFiles);
}

Utils::FilePaths ComponentImporter::createUniqueFilesList(const ProjectExplorer::Project *project,
                                                          const Utils::FilePaths &newFiles)
{
    Utils::FilePaths uniqueFiles;
    const auto projectFiles = project->files(ProjectExplorer::Project::SourceFiles);

    for (const auto &file : newFiles)
        if (!projectFiles.contains(file))
            uniqueFiles.append(file);

    return uniqueFiles;
}

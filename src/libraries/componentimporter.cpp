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

#include "componentimporter.h"

#include <QFile>

#include <projectexplorer/session.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

using namespace Asn1Acn::Internal::Libraries;

void ComponentImporter::setFiles(const QStringList &files)
{
    m_sourceFiles = files;
}

const QStringList &ComponentImporter::files() const
{
    return m_sourceFiles;
}

void ComponentImporter::setDirectory(const QString &path)
{
    m_sourceDir = QDir(path);
}

void ComponentImporter::import()
{
    const auto project = ProjectExplorer::SessionManager::startupProject();

    m_projectDir = QDir(project->projectDirectory().toString());
    m_targetDir = QDir(m_projectDir.filePath(m_sourceDir.dirName()));

    m_projectDir.mkpath(m_sourceDir.dirName());

    const auto copied = copyFilesToProject();
    addFilesToProject(project, copied);
}

QStringList ComponentImporter::copyFilesToProject()
{
    QStringList copiedFiles;

    for (const auto &file : m_sourceFiles) {
        QString target = targetFileName(file);

        m_targetDir.mkpath(QFileInfo(target).absolutePath());

        QFile::copy(file, m_targetDir.filePath(target));
        copiedFiles.append(m_projectDir.relativeFilePath(target));
    }

    return copiedFiles;
}

QString ComponentImporter::targetFileName(const QString &file)
{
    QString relativeSrcPath = m_sourceDir.relativeFilePath(file);
    return m_targetDir.filePath(relativeSrcPath);
}

void ComponentImporter::addFilesToProject(const ProjectExplorer::Project *project, const QStringList &files)
{
    const auto uniqueFiles = createUniqueFilesList(project, files);
    project->rootProjectNode()->addFiles(uniqueFiles);
}

QStringList ComponentImporter::createUniqueFilesList(const ProjectExplorer::Project *project, const QStringList &newFiles)
{
    QStringList uniqueFiles;
    const auto projectFiles = project->files(ProjectExplorer::Project::SourceFiles);

    for (const auto &file : newFiles)
        if (!projectFiles.contains(m_projectDir.filePath(file)))
            uniqueFiles.append(file);

    return uniqueFiles;
}

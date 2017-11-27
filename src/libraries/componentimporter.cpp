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

#include <QDir>

#include <projectexplorer/session.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include <utils/qtcassert.h>

using namespace Asn1Acn::Internal::Libraries;

void ComponentImporter::setFiles(const QStringList &files)
{
    m_files = files;
}

void ComponentImporter::setDirectories(const QStringList &paths)
{
    m_files = pathsInDirectory(paths);
}

const QStringList &ComponentImporter::files() const
{
    return m_files;
}

void ComponentImporter::import() const
{
    addPathsToProject(m_files);
}

QStringList ComponentImporter::pathsInDirectory(const QStringList &directories)
{
    QStringList files;
    QStringList filters;
    filters << "*.asn" << "*.asn1" << "*.acn";

    for(const auto &directory : directories) {
        QDir dir(directory);
        if (!dir.exists())
            continue;

        dir.setNameFilters(filters);
        for (const auto &path : dir.entryList())
            files << dir.filePath(path);
    }

    return files;
}

void ComponentImporter::addPathsToProject(const QStringList &paths) const
{
    const auto project = ProjectExplorer::SessionManager::startupProject();
    if (project == nullptr)
        return;

    project->rootProjectNode()->addFiles(paths);
}

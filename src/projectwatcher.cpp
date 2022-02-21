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

#include "projectwatcher.h"

#include <QSet>
#include <QString>

#include <projectexplorer/projectnodes.h>
#include <projectexplorer/session.h>
#include <utils/mimetypes/mimedatabase.h>

#include <utils/qtcassert.h>

#include "asn1acnconstants.h"
#include "projectcontenthandler.h"

using namespace Asn1Acn::Internal;

ProjectWatcher::ProjectWatcher()
{
    ProjectExplorer::SessionManager *sessionManager = ProjectExplorer::SessionManager::instance();
    connect(sessionManager,
            &ProjectExplorer::SessionManager::projectRemoved,
            this,
            &ProjectWatcher::onProjectRemoved);
    connect(sessionManager,
            &ProjectExplorer::SessionManager::projectAdded,
            this,
            &ProjectWatcher::onProjectAdded);
}

void ProjectWatcher::onProjectAdded(ProjectExplorer::Project *project)
{
    ProjectContentHandler *proc = ProjectContentHandler::create();
    proc->handleProjectAdded(project->displayName());

    connect(project,
            &ProjectExplorer::Project::fileListChanged,
            this,
            &ProjectWatcher::onProjectFileListChanged);
}

void ProjectWatcher::onProjectRemoved(ProjectExplorer::Project *project)
{
    ProjectContentHandler *proc = ProjectContentHandler::create();
    proc->handleProjectRemoved(project->displayName());

    disconnect(project,
               &ProjectExplorer::Project::fileListChanged,
               this,
               &ProjectWatcher::onProjectFileListChanged);
}

void ProjectWatcher::onProjectFileListChanged()
{
    ProjectExplorer::Project *project = qobject_cast<ProjectExplorer::Project *>(sender());
    QTC_ASSERT(project != nullptr, return );

    const QSet<QString> watchedMimeTypes{Constants::ASN1_MIMETYPE, Constants::ACN_MIMETYPE};
    const auto validFiles = project->files([&watchedMimeTypes](const ProjectExplorer::Node *n) {
        if (const auto fn = n->asFileNode())
            return watchedMimeTypes.contains(
                Utils::mimeTypeForFile(fn->filePath().toString()).name());
        return false;
    });

    ProjectContentHandler *proc = ProjectContentHandler::create();
    proc->handleFileListChanged(project->displayName(), validFiles);
}

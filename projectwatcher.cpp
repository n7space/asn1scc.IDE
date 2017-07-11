/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "projectwatcher.h"

#include <QString>

#include <projectexplorer/session.h>

#include <utils/qtcassert.h>

#include "projectcontenthandler.h"

using namespace Asn1Acn::Internal;

ProjectWatcher::ProjectWatcher()
{
    ProjectExplorer::SessionManager *sessionManager = ProjectExplorer::SessionManager::instance();
    connect(sessionManager, &ProjectExplorer::SessionManager::projectRemoved,
            this, &ProjectWatcher::onProjectRemoved);
    connect(sessionManager, &ProjectExplorer::SessionManager::projectAdded,
            this, &ProjectWatcher::onProjectAdded);
}

void ProjectWatcher::onProjectAdded(ProjectExplorer::Project *project)
{
    ProjectContentHandler *proc = new ProjectContentHandler;
    proc->handleProjectAdded(project->displayName());

    connect(project, &ProjectExplorer::Project::fileListChanged,
            this, &ProjectWatcher::onProjectFileListChanged);
}

void ProjectWatcher::onProjectRemoved(ProjectExplorer::Project *project)
{
    ProjectContentHandler *proc = new ProjectContentHandler;
    proc->handleProjectRemoved(project->displayName());

    disconnect(project, &ProjectExplorer::Project::fileListChanged,
               this, &ProjectWatcher::onProjectFileListChanged);
}

void ProjectWatcher::onProjectFileListChanged()
{
    ProjectExplorer::Project *project = qobject_cast<ProjectExplorer::Project *>(sender());
    QTC_ASSERT(project != nullptr, return );

    ProjectContentHandler *proc = new ProjectContentHandler;
    proc->handleFileListChanged(project->displayName(),
                                project->files(ProjectExplorer::Project::AllFiles));
}


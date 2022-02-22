/****************************************************************************
**
** Copyright (C) 2018-2022 N7 Space sp. z o. o.
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

#include "toolsmenuimportitemcontroller.h"

#include <QAction>

#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>

#include <utils/parameteraction.h>

using namespace Asn1Acn::Internal;

ToolsMenuImportItemController::ToolsMenuImportItemController(Utils::ParameterAction *menuItem,
                                                             QObject *parent)
    : QObject(parent)
    , m_menuItem(menuItem)
    , m_currentProject(nullptr)
{
    connect(ProjectExplorer::SessionManager::instance(),
            &ProjectExplorer::SessionManager::startupProjectChanged,
            this,
            &ToolsMenuImportItemController::onActiveProjectChanged);

    connect(ProjectExplorer::SessionManager::instance(),
            &ProjectExplorer::SessionManager::aboutToRemoveProject,
            this,
            &ToolsMenuImportItemController::onAboutToRemoveProject);
}

void ToolsMenuImportItemController::onActiveProjectChanged(ProjectExplorer::Project *project)
{
    refreshCurrentProject(project);
    if (m_currentProject == nullptr) {
        m_menuItem->setParameter(QString());
        return;
    }

    m_menuItem->setParameter(m_currentProject->displayName());

    const auto projectNode = m_currentProject->rootProjectNode();
    if (projectNode == nullptr)
        connect(project,
                &ProjectExplorer::Project::anyParsingFinished,
                this,
                &ToolsMenuImportItemController::onProjectLoadingFinished);

    updateMenuItemState(projectNode);
}

void ToolsMenuImportItemController::onAboutToRemoveProject(ProjectExplorer::Project *project)
{
    if (project == m_currentProject)
        refreshCurrentProject(nullptr);
}

void ToolsMenuImportItemController::onProjectLoadingFinished()
{
    updateMenuItemState(m_currentProject->rootProjectNode());
}

void ToolsMenuImportItemController::refreshCurrentProject(ProjectExplorer::Project *project)
{
    if (m_currentProject != nullptr) {
        disconnect(m_currentProject,
                   &ProjectExplorer::Project::anyParsingFinished,
                   this,
                   &ToolsMenuImportItemController::onProjectLoadingFinished);
        m_menuItem->setEnabled(false);
    }

    m_currentProject = project;
}

void ToolsMenuImportItemController::updateMenuItemState(const ProjectExplorer::ProjectNode *node)
{
    if (node == nullptr)
        return;

    m_menuItem->setEnabled(node->supportsAction(ProjectExplorer::AddExistingFile, node));
}

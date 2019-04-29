/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
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

#include "projectmenuimportitemcontroller.h"

#include <QAction>

#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/projecttree.h>

using namespace Asn1Acn::Internal;

ProjectMenuImportItemController::ProjectMenuImportItemController(QAction *menuItem, QObject *parent)
    : QObject(parent)
    , m_menuItem(menuItem)
{
    connect(ProjectExplorer::ProjectTree::instance(),
            &ProjectExplorer::ProjectTree::aboutToShowContextMenu,
            this,
            &ProjectMenuImportItemController::onAboutToShowContextMenu);
}

void ProjectMenuImportItemController::onAboutToShowContextMenu(ProjectExplorer::Project *project,
                                                               ProjectExplorer::Node *node)
{
    if (project == nullptr || node == nullptr)
        return;
    m_menuItem->setEnabled(node->supportsAction(ProjectExplorer::AddExistingFile, node));
}

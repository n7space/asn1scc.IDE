/****************************************************************************
**
** Copyright (C) 2018-2024 N7 Space sp. z o. o.
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

#include <QObject>

namespace Utils {
class ParameterAction;
}

namespace ProjectExplorer {
class Project;
class ProjectNode;
} // namespace ProjectExplorer

namespace Asn1Acn {
namespace Internal {

class ToolsMenuImportItemController : public QObject
{
    Q_OBJECT

public:
    ToolsMenuImportItemController(Utils::ParameterAction *menuItem, QObject *parent = nullptr);

private slots:
    void onActiveProjectChanged(ProjectExplorer::Project *project);
    void onProjectLoadingFinished();

    void onAboutToRemoveProject(ProjectExplorer::Project *project);

private:
    void refreshCurrentProject(ProjectExplorer::Project *project);
    void updateMenuItemState(const ProjectExplorer::ProjectNode *node);

    Utils::ParameterAction *m_menuItem;
    ProjectExplorer::Project *m_currentProject;
};

} // namespace Internal
} // namespace Asn1Acn

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
#include "asn1acnbuildsteprunner.h"

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>

#include "asn1acnbuildstep.h"

using namespace Asn1Acn::Internal;

void Asn1AcnBuildStepRunner::run(ProjectExplorer::Project *project)
{
    auto *step = m_cache.get(project->displayName());
    if (step == nullptr && (step = newStep(project)) == nullptr)
        return;

    ProjectExplorer::BuildManager::appendStep(step, progressLabelText());
}

Asn1AcnBuildStep *Asn1AcnBuildStepRunner::newStep(const ProjectExplorer::Project *project)
{
    const auto target = project->activeTarget();
    if (target == nullptr)
        return nullptr;

    auto buildConfig = target->activeBuildConfiguration();
    if (buildConfig == nullptr)
        return nullptr;

    auto stepsList = buildConfig->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
    auto step = createStep(stepsList);

    m_cache.add(project->displayName(), step);

    return step;
}

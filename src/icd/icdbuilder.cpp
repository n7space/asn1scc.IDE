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

#include "icdbuilder.h"

#include <QObject>
#include <QStringList>

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/makestep.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>

#include <kitinformation.h>

#include <utils/mimetypes/mimedatabase.h>

#include "asn1acnconstants.h"

using namespace Asn1Acn::Internal::Icd;
using namespace ProjectExplorer;

IcdBuilder::IcdBuilder() {}
IcdBuilder::~IcdBuilder() {}

std::shared_ptr<BuildConfiguration> IcdBuilder::config(Project *project)
{
    const auto target = project->activeTarget();
    if (!target)
        return nullptr;

    const auto buildConfig = target->activeBuildConfiguration();
    const auto factory = IBuildConfigurationFactory::find(target);

    if (!factory || !buildConfig)
        return nullptr;

    return std::shared_ptr<BuildConfiguration>{factory->clone(target, buildConfig)};
}

void IcdBuilder::run(ProjectExplorer::Project *project)
{
    auto bc = config(project);
    if (!bc)
        return; // TODO ?

    auto steps = bc->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
    auto makeStep = steps ? steps->firstOfType<MakeStep>() : nullptr;
    if (!makeStep)
        return; // TODO

    makeStep->setBuildTarget("icdFromAsn1", true);

    BuildManager::buildList(steps);

    QObject::connect(BuildManager::instance(),
                     &BuildManager::buildQueueFinished,
                     [bc](bool) mutable { bc.reset(); });
}

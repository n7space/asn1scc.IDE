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

#include <QDesktopServices>
#include <QObject>
#include <QStringList>

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/makestep.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>

#include <cmakeprojectmanager/cmakebuildstep.h>

using namespace Asn1Acn::Internal::Icd;
using namespace ProjectExplorer;

namespace {

const QString GENERATED_ICD_PREFIX = "Generated ICD: ";
const QString CMAKE_TARGETS_KEY = "CMakeProjectManager.MakeStep.BuildTargets";

void openGeneratedIcd(const QString &line)
{
    const auto icdHtml = line.mid(GENERATED_ICD_PREFIX.length()).trimmed();
    QDesktopServices::openUrl(QUrl::fromLocalFile(icdHtml));
}

bool icdGenerationFinished(const QString &line)
{
    return line.startsWith(GENERATED_ICD_PREFIX);
}

BuildStep *modifiedMakeBuildStep(BuildStepList *steps)
{
    auto step = steps->firstOfType<MakeStep>();
    if (!step)
        return nullptr;
    step->setBuildTarget("icdFromAsn1", true);
    return step;
}

BuildStep *modifiedCMakeBuildStep(BuildStepList *steps)
{
    for (auto step : steps->steps()) {
        auto map = step->toMap();
        if (map.contains(CMAKE_TARGETS_KEY)) {
            map[CMAKE_TARGETS_KEY] = "icdFromAsn1";
            step->fromMap(map);
            return step;
        }
    }
    return nullptr;
}

BuildStep *modifiedBuildStep(BuildStepList *steps)
{
    auto makeStep = modifiedMakeBuildStep(steps);
    if (makeStep != nullptr)
        return makeStep;
    return modifiedCMakeBuildStep(steps);
}

} // namespace

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
    auto buildConfig = config(project);
    auto steps = buildConfig
                     ? buildConfig->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD))
                     : nullptr;
    auto step = steps ? modifiedBuildStep(steps) : nullptr;

    if (!step)
        return; // TODO

    QObject::connect(BuildManager::instance(),
                     &BuildManager::buildQueueFinished,
                     [buildConfig](bool) mutable { buildConfig.reset(); });
    QObject::connect(step,
                     &BuildStep::addOutput,
                     [](const QString &string,
                        BuildStep::OutputFormat,
                        BuildStep::OutputNewlineSetting) {
                         if (icdGenerationFinished(string))
                             openGeneratedIcd(string);
                     });

    BuildManager::buildList(steps);
}

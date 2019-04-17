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
#include <QMessageBox>
#include <QObject>
#include <QStringList>

#include <coreplugin/icore.h>

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/makestep.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>

#include <cmakeprojectmanager/cmakebuildstep.h>

#include <asn1acn.h>

using namespace Asn1Acn::Internal::Icd;
using namespace Asn1Acn::Internal;
using namespace ProjectExplorer;

namespace {

const QString GENERATED_ICD_PREFIX = "Generated ICD: ";
const QString CMAKE_TARGETS_KEY = "CMakeProjectManager.MakeStep.BuildTargets";
const QString ICD_TARGET_NAME = "icdFromAsn1";

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
    if (step == nullptr)
        return nullptr;
    step->setBuildTarget(ICD_TARGET_NAME, true);
    return step;
}

BuildStep *modifiedCMakeBuildStep(BuildStepList *steps)
{
    for (auto step : steps->steps()) {
        auto map = step->toMap();
        if (map.contains(CMAKE_TARGETS_KEY)) {
            map[CMAKE_TARGETS_KEY] = ICD_TARGET_NAME;
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

std::shared_ptr<BuildConfiguration> wrapForProperDeletion(BuildConfiguration *buildConfig)
{
    auto result = std::shared_ptr<BuildConfiguration>{buildConfig};
    QObject::connect(BuildManager::instance(),
                     &BuildManager::buildQueueFinished,
                     [result](bool) mutable { result.reset(); });
    return result;
}

std::shared_ptr<BuildConfiguration> cloneActiveConfiguration(Project *project)
{
    const auto target = project->activeTarget();
    if (target == nullptr)
        return nullptr;

    const auto buildConfig = target->activeBuildConfiguration();
    const auto factory = BuildConfigurationFactory::find(target);

    if (factory == nullptr || buildConfig == nullptr)
        return nullptr;

    return wrapForProperDeletion(factory->clone(target, buildConfig));
}

BuildStepList *cloneBuildStepsFrom(Project *project)
{
    auto buildConfig = cloneActiveConfiguration(project);
    if (buildConfig == nullptr)
        return nullptr;
    return buildConfig->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void displayWarning()
{
    QMessageBox::warning(Core::ICore::dialogParent(),
                         Asn1AcnPlugin::tr("Build ICD failed"),
                         Asn1AcnPlugin::tr(
                             "Incorrect project configuration - command supported only in "
                             "qmake and cmake project created by plugin."),
                         QMessageBox::Ok);
}

void addStepOutputListener(BuildStep *step)
{
    QObject::connect(step,
                     &BuildStep::addOutput,
                     [](const QString &string,
                        BuildStep::OutputFormat,
                        BuildStep::OutputNewlineSetting) {
                         if (icdGenerationFinished(string))
                             openGeneratedIcd(string);
                     });
}

} // namespace

IcdBuilder::IcdBuilder() {}
IcdBuilder::~IcdBuilder() {}

void IcdBuilder::run(ProjectExplorer::Project *project)
{
    auto steps = cloneBuildStepsFrom(project);
    auto step = (steps != nullptr) ? modifiedBuildStep(steps) : nullptr;

    if (step == nullptr) {
        displayWarning();
        return;
    }

    addStepOutputListener(step);

    BuildManager::buildList(steps);
}

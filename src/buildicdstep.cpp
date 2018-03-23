/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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

#include "buildicdstep.h"

#include <QRegularExpression>
#include <QStringList>

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/processparameters.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>

#include <kitinformation.h>

#include <utils/qtcassert.h>

#include <icderrorparser.h>
#include <icdstepscache.h>

using namespace Asn1Acn::Internal;

static const char ICD_BS_ID[] = "ASN1ACN.BuildICDStep";
static QString OUTPUT_PATH(QDir::toNativeSeparators("asn1sccGenerated/icd/"));

static BuildICDStep *createStep(const ProjectExplorer::Project *project, ICDStepsCache &cache)
{
    const auto target = project->activeTarget();
    if (target == nullptr)
        return nullptr;

    auto buildConfig = target->activeBuildConfiguration();
    if (buildConfig == nullptr)
        return nullptr;

    auto step = new BuildICDStep(
        buildConfig->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD)));
    cache.add(project->displayName(), step);

    return step;
}

void ICDBuilder::runBuild(ProjectExplorer::Project *project)
{
    QTC_ASSERT(project != nullptr, return );

    static ICDStepsCache cache;

    auto *step = cache.get(project->displayName());
    if (step == nullptr && (step = createStep(project, cache)) == nullptr)
        return;

    ProjectExplorer::BuildManager::appendStep(step, QLatin1String("ICD"));
}

BuildICDStep::BuildICDStep(ProjectExplorer::BuildStepList *parent)
    : AbstractProcessStep(parent, Core::Id(ICD_BS_ID))
    , m_commandFuture(nullptr)
    , m_outputFilename("icd.html")
{
    setDefaultDisplayName(tr("icd"));

    connect(&m_inputWatcher, &QFutureWatcher<bool>::canceled, this, [this]() {
        if (m_commandFuture)
            m_commandFuture->cancel();
    });

    connect(&m_commandWatcher, &QFutureWatcher<bool>::finished, this, &BuildICDStep::onFinish);
}

bool BuildICDStep::init(QList<const BuildStep *> &earlierSteps)
{
    if (m_commandFuture || !updateRunParams()) {
        addOutput(tr("Build initialization failed"), BuildStep::OutputFormat::ErrorMessage);
        return false;
    }

    setOutputParser(new ICDErrorParser);

    return AbstractProcessStep::init(earlierSteps);
}

bool BuildICDStep::updateRunParams()
{
    const auto bc = buildConfiguration();

    updateEnvironment(bc);

    return updateOutputDirectory(bc) && updateAsn1SccCommand() && updateSourcesList();
}

void BuildICDStep::updateEnvironment(const ProjectExplorer::BuildConfiguration *bc)
{
    auto *pp = processParameters();
    pp->setEnvironment(bc->environment());
}

bool BuildICDStep::updateOutputDirectory(const ProjectExplorer::BuildConfiguration *bc)
{
    m_outputPath = bc->buildDirectory().appendPath(OUTPUT_PATH).toString();

    if (!QDir::root().mkpath(m_outputPath)) {
        addOutput(tr("Could not create ICD output directory ") + m_outputPath,
                  BuildStep::OutputFormat::ErrorMessage);
        return false;
    }

    return true;
}

bool BuildICDStep::updateAsn1SccCommand()
{
    m_asn1sccCommand = KitInformation::hasAsn1Exe(target()->kit())
                           ? KitInformation::asn1Exe(target()->kit()).toString()
                           : QString();

    if (m_asn1sccCommand.isEmpty()) {
        addOutput(tr("Asn1scc compiler is not defined"), BuildStep::OutputFormat::ErrorMessage);
        return false;
    }

    return true;
}

bool BuildICDStep::updateSourcesList()
{
    const auto project = target()->project();

    static const QRegularExpression re("\\.asn1?$");
    m_sources = project->files(ProjectExplorer::Project::SourceFiles).filter(re);

    if (m_sources.isEmpty()) {
        addOutput(tr("No ASN.1 present in project"), BuildStep::OutputFormat::ErrorMessage);
        return false;
    }

    return true;
}

void BuildICDStep::run(QFutureInterface<bool> &f)
{
    emit addOutput(tr("Running ICD build"), BuildStep::OutputFormat::NormalMessage);

    updateInput(f);
    updateProcess(m_asn1sccCommand, argument());
    updateCommand();

    AbstractProcessStep::run(*m_commandFuture);
}

void BuildICDStep::updateInput(QFutureInterface<bool> &f)
{
    m_inputFuture = f;
    m_inputFuture.setProgressRange(0, 1);
    m_inputFuture.setProgressValue(0);
    m_inputWatcher.setFuture(m_inputFuture.future());
}

void BuildICDStep::updateProcess(const QString &command, const QString &arg)
{
    auto *pp = processParameters();
    pp->setCommand(command);
    pp->setArguments(arg);
    pp->resolveAll();
}

void BuildICDStep::updateCommand()
{
    QTC_ASSERT(!m_commandFuture || m_commandFuture->future().isFinished(), return );
    m_commandFuture.reset(new QFutureInterface<bool>);
    m_commandWatcher.setFuture(m_commandFuture->future());
}

void BuildICDStep::onFinish()
{
    bool success = finishCommand();
    finishInput(success);
}

bool BuildICDStep::finishCommand()
{
    bool success = false;
    if (m_commandFuture->isCanceled())
        emit addOutput(tr("ICD build cancelled"), BuildStep::OutputFormat::ErrorMessage);
    else if (m_commandFuture->isFinished())
        success = m_commandFuture->future().result();

    m_commandFuture.reset();

    return success;
}

void BuildICDStep::finishInput(bool success)
{
    m_inputFuture.setProgressValue(1);
    reportRunResult(m_inputFuture, success);

    m_inputFuture = QFutureInterface<bool>();
}

QString BuildICDStep::argument()
{
    QString files = m_sources.join(QLatin1Char(' '));
    return files + QString(" -icdAcn ") + m_outputPath + m_outputFilename;
}

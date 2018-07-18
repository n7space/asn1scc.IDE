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

#include "asn1acnbuildstep.h"

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/processparameters.h>

#include <utils/qtcassert.h>

#include "asn1acnerrorparser.h"

using namespace Asn1Acn::Internal;

Asn1AcnBuildStep::Asn1AcnBuildStep(ProjectExplorer::BuildStepList *parent,
                                   const char *id,
                                   const QString &displayName)
    : AbstractProcessStep(parent, Core::Id(id))
    , m_commandFuture(nullptr)
{
    setDefaultDisplayName(displayName);

    connect(&m_inputWatcher, &QFutureWatcher<bool>::canceled, this, [this]() {
        if (m_commandFuture)
            m_commandFuture->cancel();
    });

    connect(&m_commandWatcher, &QFutureWatcher<bool>::finished, this, &Asn1AcnBuildStep::onFinish);
}

bool Asn1AcnBuildStep::init(QList<const BuildStep *> &earlierSteps)
{
    if (m_commandFuture) {
        addOutput(tr("Command already running"), BuildStep::OutputFormat::ErrorMessage);
        return false;
    }

    updateEnvironment(buildConfiguration());

    setOutputParser(new Asn1AcnErrorParser);

    return AbstractProcessStep::init(earlierSteps);
}

void Asn1AcnBuildStep::run(QFutureInterface<bool> &f)
{
    addOutput(tr("Starting Asn1Scc build step"), BuildStep::OutputFormat::NormalMessage);

    updateInput(f);
    updateProcess(executablePath(), arguments());
    updateCommand();

    AbstractProcessStep::run(*m_commandFuture);
}

void Asn1AcnBuildStep::updateEnvironment(const ProjectExplorer::BuildConfiguration *bc)
{
    auto *pp = processParameters();
    pp->setEnvironment(bc->environment());
}

void Asn1AcnBuildStep::updateInput(QFutureInterface<bool> &f)
{
    m_inputFuture = f;
    m_inputFuture.setProgressRange(0, 1);
    m_inputFuture.setProgressValue(0);
    m_inputWatcher.setFuture(m_inputFuture.future());
}

void Asn1AcnBuildStep::updateProcess(const QString &command, const QString &arg)
{
    auto *pp = processParameters();
    pp->setCommand(command);
    pp->setArguments(arg);
    pp->resolveAll();
}

void Asn1AcnBuildStep::updateCommand()
{
    QTC_ASSERT(!m_commandFuture || m_commandFuture->future().isFinished(), return );
    m_commandFuture.reset(new QFutureInterface<bool>);
    m_commandWatcher.setFuture(m_commandFuture->future());
}

void Asn1AcnBuildStep::onFinish()
{
    bool success = finishCommand();
    finishInput(success);
}

bool Asn1AcnBuildStep::finishCommand()
{
    bool success = false;
    if (m_commandFuture->isCanceled())
        addOutput(tr("Asn1Acn build step cancelled"), BuildStep::OutputFormat::ErrorMessage);
    else if (m_commandFuture->isFinished())
        success = m_commandFuture->future().result();

    m_commandFuture.reset();

    return success;
}

void Asn1AcnBuildStep::finishInput(bool success)
{
    m_inputFuture.setProgressValue(1);
    reportRunResult(m_inputFuture, success);

    m_inputFuture = QFutureInterface<bool>();
}

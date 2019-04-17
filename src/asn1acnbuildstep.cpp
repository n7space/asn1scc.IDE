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
{
    setDefaultDisplayName(displayName);
}

bool Asn1AcnBuildStep::init()
{
    updateEnvironment(buildConfiguration());
    setOutputParser(new Asn1AcnErrorParser);
    return AbstractProcessStep::init();
}

void Asn1AcnBuildStep::doRun()
{
    addOutput(tr("Starting Asn1Scc build step"), BuildStep::OutputFormat::NormalMessage);
    updateProcess(executablePath(), arguments());
    AbstractProcessStep::doRun();
}

void Asn1AcnBuildStep::updateEnvironment(const ProjectExplorer::BuildConfiguration *bc)
{
    auto *pp = processParameters();
    pp->setEnvironment(bc->environment());
}

void Asn1AcnBuildStep::updateProcess(const QString &command, const QString &arg)
{
    auto *pp = processParameters();
    pp->setCommand(command);
    pp->setArguments(arg);
    pp->resolveAll();
}

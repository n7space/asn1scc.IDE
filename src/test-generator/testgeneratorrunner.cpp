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

#include "testgeneratorrunner.h"

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/processparameters.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>

#include <kitinformation.h>

#include <utils/mimetypes/mimedatabase.h>
#include <utils/qtcassert.h>

#include "asn1acnconstants.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TestGenerator;

static const char GENERATE_TESTS_BS_ID[] = "ASN1ACN.TestGeneratorStep";

TestGeneratorRunner::TestGeneratorRunner(const TestGeneratorParamsProviderPtr params)
    : m_params(params)
{}

Asn1AcnBuildStep *TestGeneratorRunner::createStep(ProjectExplorer::BuildStepList *stepList) const
{
    return new TestGeneratorStep(stepList, m_params);
}

QString TestGeneratorRunner::progressLabelText() const
{
    return QStringLiteral("Test Generator");
}

TestGeneratorStep::TestGeneratorStep(ProjectExplorer::BuildStepList *parent,
                                     const TestGeneratorParamsProviderPtr params)
    : Asn1AcnBuildStep(parent, GENERATE_TESTS_BS_ID, QStringLiteral("Test Generator"))
    , m_params(params)
{}

QString TestGeneratorStep::executablePath() const
{
    return m_params->testGeneratorPath();
}

QString TestGeneratorStep::arguments() const
{
    return asn1sccArgument() + mainStructureArgument() + outputPathArgument() + fileArgument();
}

QString TestGeneratorStep::asn1sccArgument() const
{
    auto path = m_params->asn1sccPath(target());
    if (path.isEmpty())
        return {};

    return QStringLiteral("--asn1scc-path ") + path + QChar(' ');
}

QString TestGeneratorStep::mainStructureArgument() const
{
    auto mainStruct = m_params->mainStructureName();
    QTC_ASSERT(!mainStruct.isEmpty(), return {});

    return QStringLiteral("--main-structure ") + mainStruct + QChar(' ');
}

QString TestGeneratorStep::outputPathArgument() const
{
    auto outputPath = m_params->outputPath();
    QTC_ASSERT(!outputPath.isEmpty(), return {});

    return QStringLiteral("--output-dir ") + outputPath + QChar(' ');
}

QString TestGeneratorStep::fileArgument() const
{
    const auto project = target()->project();

    auto sources = project->files([](const ProjectExplorer::Node *n) {
        if (const auto fn = n->asFileNode()) {
            auto name = Utils::mimeTypeForFile(fn->filePath().toString()).name();
            return name == Constants::ASN1_MIMETYPE || name == Constants::ACN_MIMETYPE;
        }
        return false;
    });

    QString ret;
    for (const auto &source : sources)
        ret += source.toString() + QChar(' ');

    return ret + QChar(' ');
}

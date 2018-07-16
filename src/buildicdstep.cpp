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

#include <QStringList>

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/processparameters.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/target.h>

#include <kitinformation.h>

#include <utils/mimetypes/mimedatabase.h>
#include <utils/qtcassert.h>

#include <icderrorparser.h>

#include "asn1acnconstants.h"
#include "asn1acnstepscache.h"

using namespace Asn1Acn::Internal;

static const char ICD_BS_ID[] = "ASN1ACN.BuildICDStep";
static QString OUTPUT_PATH(QDir::toNativeSeparators("asn1sccGenerated/icd/"));

Asn1AcnBuildStep *ICDBuilder::createStep(ProjectExplorer::BuildStepList *stepList) const
{
    return new BuildICDStep(stepList);
}

QString ICDBuilder::progressLabelText() const
{
    return QStringLiteral("icd");
}

BuildICDStep::BuildICDStep(ProjectExplorer::BuildStepList *parent)
    : Asn1AcnBuildStep(parent, ICD_BS_ID, QStringLiteral("icd"))
    , m_outputFilename("icd.html")
{}

bool BuildICDStep::init(QList<const BuildStep *> &earlierSteps)
{
    if (!updateRunParams()) {
        addOutput(tr("Could not initialize build params"), BuildStep::OutputFormat::ErrorMessage);
        return false;
    }

    setOutputParser(new ICDErrorParser);

    return Asn1AcnBuildStep::init(earlierSteps);
}

bool BuildICDStep::updateRunParams()
{
    return updateOutputDirectory(buildConfiguration()) && updateAsn1SccCommand()
           && updateSourcesList();
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

    m_sources = project->files([](const ProjectExplorer::Node *n) {
        if (const auto fn = n->asFileNode())
            return Utils::mimeTypeForFile(fn->filePath().toString()).name()
                   == Constants::ASN1_MIMETYPE;
        return false;
    });

    if (m_sources.isEmpty()) {
        addOutput(tr("No ASN.1 present in project"), BuildStep::OutputFormat::ErrorMessage);
        return false;
    }

    return true;
}

namespace {
QString quoteOne(const QString &file)
{
    return QLatin1Char('"') + file + QLatin1Char('"');
}

QStringList quoteList(const Utils::FileNameList &files)
{
    QStringList res;
    for (const auto &file : files)
        res << quoteOne(file.toString());
    return res;
}
} // namespace

QString BuildICDStep::arguments() const
{
    const auto in = quoteList(m_sources).join(QLatin1Char(' '));
    const auto out = quoteOne(m_outputPath + m_outputFilename);

    return QString(" -icdAcn ") + out + QString(" ") + in;
}

QString BuildICDStep::executablePath() const
{
    return m_asn1sccCommand;
}

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
#pragma once

#include "testgeneratorparamsprovider.h"

#include "asn1acnbuildstep.h"
#include "asn1acnbuildsteprunner.h"

namespace Asn1Acn {
namespace Internal {
namespace TestGenerator {

class TestGeneratorRunner : public Asn1AcnBuildStepRunner
{
public:
    TestGeneratorRunner(const TestGeneratorParamsProviderPtr params);

private:
    Asn1AcnBuildStep *createStep(ProjectExplorer::BuildStepList *stepList) const override;
    QString progressLabelText() const override;

    const TestGeneratorParamsProviderPtr m_params;
};

class TestGeneratorBuildStep : public Asn1AcnBuildStep
{
    Q_OBJECT

public:
    explicit TestGeneratorBuildStep(ProjectExplorer::BuildStepList *parent,
                                    const TestGeneratorParamsProviderPtr params);

private:
    QString executablePath() const override;
    QString arguments() const override;

    QString asn1sccArgument() const;
    QString mainStructureArgument() const;
    QString outputPathArgument() const;
    QString fileArgument() const;

    const TestGeneratorParamsProviderPtr m_params;
};

} // namespace TestGenerator
} // namespace Internal
} // namespace Asn1Acn

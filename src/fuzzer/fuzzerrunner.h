/****************************************************************************
**
** Copyright (C) 2018-2024 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include "fuzzerparamsprovider.h"

#include "asn1acnbuildstep.h"
#include "asn1acnbuildsteprunner.h"

namespace Asn1Acn {
namespace Internal {
namespace Fuzzer {

class FuzzerRunner : public Asn1AcnBuildStepRunner
{
public:
    FuzzerRunner(const FuzzerParamsProviderPtr params);

private:
    Asn1AcnBuildStep *createStep(ProjectExplorer::BuildStepList *stepList) const override;
    QString progressLabelText() const override;

    const FuzzerParamsProviderPtr m_params;
};

class FuzzerBuildStep : public Asn1AcnBuildStep
{
    Q_OBJECT

public:
    explicit FuzzerBuildStep(ProjectExplorer::BuildStepList *parent,
                             const FuzzerParamsProviderPtr params);

private:
    QString executablePath() const override;
    QString arguments() const override;

    QString asn1sccArgument() const;
    QString rootTypeArgument() const;
    QString outputPathArgument() const;
    QString fileArgument() const;

    const FuzzerParamsProviderPtr m_params;
};

} // namespace Fuzzer
} // namespace Internal
} // namespace Asn1Acn

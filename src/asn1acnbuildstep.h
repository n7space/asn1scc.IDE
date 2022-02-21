/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
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

#include <memory>

#include <projectexplorer/abstractprocessstep.h>

namespace Asn1Acn {
namespace Internal {

class Asn1AcnBuildStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT

public:
    explicit Asn1AcnBuildStep(ProjectExplorer::BuildStepList *parent,
                              const char *id,
                              const QString &displayName);

    bool init() override;
    void setupOutputFormatter(Utils::OutputFormatter *formatter) override;
    void doRun() override;

    QWidget *createConfigWidget() override { return nullptr; }

private:
    virtual QString executablePath() const = 0;
    virtual QString arguments() const = 0;

    void updateProcess(const QString &command, const QString &arg);
    void updateCommand();

    void updateEnvironment(const ProjectExplorer::BuildConfiguration *bc);
};

} // namespace Internal
} // namespace Asn1Acn

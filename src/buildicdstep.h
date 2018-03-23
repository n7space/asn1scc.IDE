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

#include <QFutureWatcher>
#include <qobjectdefs.h>

#include <projectexplorer/abstractprocessstep.h>

namespace Asn1Acn {
namespace Internal {

class ICDBuilder
{
public:
    static void runBuild(ProjectExplorer::Project *project);
};

class BuildICDStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT

public:
    explicit BuildICDStep(ProjectExplorer::BuildStepList *parent);

    bool init(QList<const BuildStep *> &earlierSteps) override;
    void run(QFutureInterface<bool> &) override;

    ProjectExplorer::BuildStepConfigWidget *createConfigWidget() override { return nullptr; }

private slots:
    void onFinish();

private:
    bool updateRunParams();
    void updateEnvironment(const ProjectExplorer::BuildConfiguration *bc);
    bool updateOutputDirectory(const ProjectExplorer::BuildConfiguration *bc);
    bool updateAsn1SccCommand();
    bool updateSourcesList();

    void updateInput(QFutureInterface<bool> &f);
    void updateProcess(const QString &command, const QString &arg);
    void updateCommand();

    bool finishCommand();
    void finishInput(bool success);

    QString argument();

    QFutureWatcher<bool> m_inputWatcher;
    QFutureInterface<bool> m_inputFuture;

    QFutureWatcher<bool> m_commandWatcher;
    std::unique_ptr<QFutureInterface<bool>> m_commandFuture;

    QString m_asn1sccCommand;
    QString m_outputPath;
    const QString m_outputFilename;
    QStringList m_sources;
};

} // namespace Internal
} // namespace Asn1Acn

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

#include <QDialog>

#include "data/file.h"

#include "asn1acnbuildsteprunner.h"

#include "testgeneratorparamsprovider.h"
#include "testgeneratorrunner.h"

namespace Asn1Acn {
namespace Internal {
namespace TestGenerator {

class TestGeneratorWidget;

class TestGeneratorParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TestGeneratorParamsDialog(TestGeneratorParamsProviderPtr provider,
                                       QWidget *parent = nullptr);

public slots:
    int exec() override;
    void accept() override;

private:
    void fillMainStructCandidates();
    void fillMainStructCandidatesFromProject(const QString &projectName);
    void fillMainStructCandidatesFromDefinitions(const Data::File::DefinitionsList &defs);

    void letProceed(bool val);

    TestGeneratorWidget *m_widget;
    TestGeneratorParamsProviderPtr m_provider;
    std::unique_ptr<Asn1AcnBuildStepRunner> m_runner;
};

} // namespace TestGenerator
} // namespace Internal
} // namespace Asn1Acn
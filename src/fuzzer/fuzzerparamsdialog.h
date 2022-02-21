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

#include <QDialog>

#include "data/file.h"

#include "asn1acnbuildsteprunner.h"

#include "fuzzerparamsprovider.h"
#include "fuzzerrunner.h"

namespace Asn1Acn {
namespace Internal {
namespace Fuzzer {

class FuzzerParamsWidget;

class FuzzerParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FuzzerParamsDialog(FuzzerParamsProviderPtr provider, QWidget *parent = nullptr);

public slots:
    int exec() override;
    void accept() override;

private:
    void fillRootTypeCandidates();
    void fillRootTypeCandidatesFromProject(const QString &projectName);
    void fillRootTypeCandidatesFromDefinitions(const Data::File::DefinitionsList &defs);

    void letProceed(bool val);

    FuzzerParamsWidget *m_widget;
    FuzzerParamsProviderPtr m_provider;
    std::unique_ptr<Asn1AcnBuildStepRunner> m_runner;
};

} // namespace Fuzzer
} // namespace Internal
} // namespace Asn1Acn

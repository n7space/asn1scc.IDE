/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
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

#include <QWizardPage>

#include "ui_import_component.h"

#include <settings/libraries.h>

#include <libraries/componentimporter.h>

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Wizard {

class SelectSourcePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SelectSourcePage(ComponentImporter &importer, QWidget *parent = nullptr);

    void initializePage() override;
    bool validatePage() override;
    bool isComplete() const override;

private slots:
    void refreshPaths();
    void builtInRadioToggled(bool checked);

private:
    bool configHasBuiltInLibraries() const;

    Ui::SelectSourcePage m_ui;
    Settings::LibrariesConstPtr m_libraries;

    ComponentImporter &m_importer;
};

} // namespace Wizard
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

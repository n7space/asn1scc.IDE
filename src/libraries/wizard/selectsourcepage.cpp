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

#include "selectsourcepage.h"

#include <QVBoxLayout>
#include <QWidget>

#include <coreplugin/icore.h>

#include <libraries/librarystorage.h>
#include <libraries/metadata/general.h>

#include "importcomponentwizard.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

SelectSourcePage::SelectSourcePage(ComponentImporter &importer, QWidget *parent)
    : QWizardPage(parent)
    , m_libraries(Settings::load<Settings::Libraries>())
    , m_importer(importer)
{
    m_ui.setupUi(this);

    setTitle(QLatin1String("Select library source"));

    connect(m_libraries.get(), &Settings::Libraries::changed, this, &SelectSourcePage::refreshPaths);
    connect(m_ui.builtInRadio, &QRadioButton::toggled, this, &SelectSourcePage::builtInRadioToggled);

    connect(m_ui.asn1sccPathChooser,
            &Utils::PathChooser::pathChanged,
            this,
            &SelectSourcePage::completeChanged);

    registerField("builtInRadio", m_ui.builtInRadio);
    registerField("customRadio", m_ui.customRadio);

    registerField("builtInCombo", m_ui.builtInCombo, "currentData");
    registerField("pathChoser", m_ui.asn1sccPathChooser, "path", "pathChanged");
}

void SelectSourcePage::initializePage()
{
    if (configHasBuiltInLibraries()) {
        m_ui.builtInRadio->setChecked(true);
    } else {
        m_ui.builtInRadio->setEnabled(false);
        m_ui.customRadio->setChecked(true);
    }
    QWizardPage::initializePage();
}

bool SelectSourcePage::validatePage()
{
    if (m_ui.builtInRadio->isChecked())
        m_importer.setDirectory(m_ui.builtInCombo->currentData().toString());
    else if (m_ui.customRadio->isChecked())
        m_importer.setDirectory(m_ui.asn1sccPathChooser->path());

    return QWizardPage::validatePage();
}

bool SelectSourcePage::isComplete() const
{
    return m_ui.builtInRadio->isChecked() || m_ui.asn1sccPathChooser->isValid();
}

void SelectSourcePage::refreshPaths()
{
    if (!m_ui.builtInRadio->isChecked())
        return;

    m_ui.builtInCombo->clear();

    for (const auto &path : m_libraries->libPaths()) {
        const auto metadata = LibraryStorage::instance()->generalMetadata(path);
        m_ui.builtInCombo->addItem(metadata.name(), metadata.path());
    }
}

void SelectSourcePage::builtInRadioToggled(bool checked)
{
    if (checked)
        refreshPaths();
    else
        m_ui.builtInCombo->clear();

    emit completeChanged();
}

bool SelectSourcePage::configHasBuiltInLibraries() const
{
    return !m_libraries->libPaths().isEmpty();
}

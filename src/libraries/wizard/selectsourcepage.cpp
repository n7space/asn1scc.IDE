/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.pl/Space
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

#include <QWidget>
#include <QVBoxLayout>

#include <coreplugin/icore.h>

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

    // TODO: signal Settings::Libraries::changed is never emitted.
    connect(m_libraries.get(), &Settings::Libraries::changed, this, &SelectSourcePage::refreshPaths);
    connect(m_ui.builtInRadio, &QRadioButton::toggled, this, &SelectSourcePage::builtInRadioToggled);

    connect(m_ui.asn1sccPathChooser, &Utils::PathChooser::pathChanged,
            this, &SelectSourcePage::completeChanged);

    m_ui.builtInRadio->setChecked(true);

    registerField("builtInRadio", m_ui.builtInRadio);
    registerField("customRadio", m_ui.customRadio);

    registerField("builtInCombo", m_ui.builtInCombo, "currentText");
    registerField("pathChoser", m_ui.asn1sccPathChooser, "path", "pathChanged");
}

bool SelectSourcePage::validatePage()
{
    if (m_ui.builtInRadio->isChecked())
        m_importer.setDirectories(QStringList(m_ui.builtInCombo->currentText()));
    else if (m_ui.customRadio->isChecked())
        m_importer.setDirectories(QStringList(m_ui.asn1sccPathChooser->path()));

    return QWizardPage::validatePage();
}

int SelectSourcePage::nextId() const
{
    if (m_ui.builtInRadio->isChecked())
        return static_cast<int>(ImportComponentWizard::Page::Page_SelectItems);

    return static_cast<int>(ImportComponentWizard::Page::Page_Summary);
}

bool SelectSourcePage::isComplete() const
{
    return !(m_ui.customRadio->isChecked() && m_ui.asn1sccPathChooser->path().isEmpty());
}

void SelectSourcePage::refreshPaths()
{
    if (!m_ui.builtInRadio->isChecked())
        return;

    m_ui.builtInCombo->clear();

    const auto detectedPaths = m_libraries->detectedLibPaths();
    const auto manualPaths = m_libraries->manualLibPaths();

    m_ui.builtInCombo->addItems(detectedPaths);
    m_ui.builtInCombo->addItems(manualPaths);
}

void SelectSourcePage::builtInRadioToggled(bool checked)
{
    if (checked)
        refreshPaths();
    else
        m_ui.builtInCombo->clear();

    emit completeChanged();
}

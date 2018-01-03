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

#include "summarypage.h"

#include <QVariant>
#include <QBoxLayout>

#include <coreplugin/iversioncontrol.h>
#include <coreplugin/vcsmanager.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

static const QString NO_VCS_DISPLAY_NAME("<None>");

SummaryPage::SummaryPage(ComponentImporter &importer, QWidget *parent)
    : QWizardPage(parent)
    , m_importer(importer)
{
    setTitle(QLatin1String("Summary"));
    m_ui.setupUi(this);    
}

void SummaryPage::initializePage()
{
    fillFilesList();
    setupVcsSelection();
}

void SummaryPage::onVcsCheckboxStateChanged(int state)
{
    setVcsComboState(state == Qt::Checked);
}

void SummaryPage::fillFilesList()
{
    QString filesText;
    for (auto it = m_importer.files().begin(); it != m_importer.files().end(); it++) {
        filesText += *it;
        if (it + 1 != m_importer.files().end())
            filesText += QLatin1String(",\n");
    }

    m_ui.filesList->setPlainText(filesText);
}

void SummaryPage::setupVcsSelection()
{
    const auto &versionControls = Core::VcsManager::versionControls();
    setVcsCheckboxState(!versionControls.isEmpty());
}

void SummaryPage::setVcsCheckboxState(bool enabled)
{
    m_ui.vcsCheckBox->setEnabled(enabled);

    if (enabled) {
        connect(m_ui.vcsCheckBox, &QCheckBox::stateChanged,
                this, &SummaryPage::onVcsCheckboxStateChanged);
    } else {
        disconnect(m_ui.vcsCheckBox, &QCheckBox::stateChanged,
                   this, &SummaryPage::onVcsCheckboxStateChanged);

        m_ui.vcsCheckBox->setChecked(false);
    }
}

void SummaryPage::setVcsComboState(bool enabled)
{
    m_ui.vcsComboBox->clear();
    m_ui.vcsComboBox->setEnabled(enabled);

    if (enabled)
        fillVcsComboItems();
}

void SummaryPage::fillVcsComboItems()
{
    m_ui.vcsComboBox->addItem(NO_VCS_DISPLAY_NAME);
    for (const auto &control : Core::VcsManager::versionControls())
        m_ui.vcsComboBox->addItem(control->displayName());
}

/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
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

#include "summarypage.h"

#include <coreplugin/iversioncontrol.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

static const QString NO_VCS_DISPLAY_NAME("<None>");

SummaryPage::SummaryPage(ComponentImporter &importer, VcsHandler &handler, QWidget *parent)
    : QWizardPage(parent)
    , m_importer(importer)
    , m_vcsHandler(handler)
{
    setTitle(QLatin1String("Summary"));
    m_ui.setupUi(this);

    connect(&m_vcsHandler, &VcsHandler::vcsListChanged, this, &SummaryPage::onVcsListChanged);
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

void SummaryPage::onVcsComboIndexChanged(int index)
{
    m_vcsHandler.setCurrentIndex(index - 1);
}

void SummaryPage::onVcsListChanged()
{
    setupVcsSelection();
}

void SummaryPage::fillFilesList()
{
    QString filesText;
    for (auto it = m_importer.sourceFiles().begin(); it != m_importer.sourceFiles().end(); it++) {
        filesText += *it;
        if (it + 1 != m_importer.sourceFiles().end())
            filesText += QLatin1String(",\n");
    }

    m_ui.filesList->setPlainText(filesText);
}

void SummaryPage::setupVcsSelection()
{
    setVcsCheckboxState(!m_vcsHandler.vcsList().isEmpty());
}

void SummaryPage::setVcsCheckboxState(bool enabled)
{
    m_ui.vcsCheckBox->setEnabled(enabled);
    setVcsComboState(enabled && m_ui.vcsCheckBox->checkState() == Qt::Checked);

    if (enabled)
        connect(m_ui.vcsCheckBox,
                &QCheckBox::stateChanged,
                this,
                &SummaryPage::onVcsCheckboxStateChanged);
    else
        disconnect(m_ui.vcsCheckBox,
                   &QCheckBox::stateChanged,
                   this,
                   &SummaryPage::onVcsCheckboxStateChanged);
}

void SummaryPage::setVcsComboState(bool enabled)
{
    m_ui.vcsComboBox->clear();
    m_ui.vcsComboBox->setEnabled(enabled);

    if (enabled) {
        fillVcsComboItems();
        connect(m_ui.vcsComboBox,
                static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this,
                &SummaryPage::onVcsComboIndexChanged);
    } else {
        disconnect(m_ui.vcsComboBox,
                   static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                   this,
                   &SummaryPage::onVcsComboIndexChanged);
    }
}

void SummaryPage::fillVcsComboItems()
{
    m_ui.vcsComboBox->addItem(NO_VCS_DISPLAY_NAME);
    for (const auto &item : m_vcsHandler.vcsList())
        m_ui.vcsComboBox->addItem(item->displayName());
}

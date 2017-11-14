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

#include "importcomponentdialog.h"

#include <QWidget>

#include <coreplugin/icore.h>

using namespace Asn1Acn::Internal::Libraries;

ComponentImporter ImportComponentDialog::m_importer;

ImportComponentDialog::ImportComponentDialog(QWidget *parent)
    : QDialog(parent)
    , m_libraries(Settings::load<Settings::Libraries>())
{
    m_ui.setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    connect(Core::ICore::instance(), &Core::ICore::coreAboutToClose, this, &QWidget::close);

    // TODO: signal Settings::Libraries::changed is never emitted.
    connect(m_libraries.get(), &Settings::Libraries::changed, this, &ImportComponentDialog::refreshPaths);
    connect(m_ui.builtInRadio, &QRadioButton::toggled, this, &ImportComponentDialog::builtInRadioToggled);

    connect(m_ui.buttonBox, &QDialogButtonBox::accepted, this, &ImportComponentDialog::accept);
    connect(m_ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    m_ui.builtInRadio->setChecked(true);
}

void ImportComponentDialog::refreshPaths()
{
    if (!m_ui.builtInRadio->isChecked())
        return;

    m_ui.builtInCombo->clear();

    const auto detectedPaths = m_libraries->detectedLibPaths();
    const auto manualPaths = m_libraries->manualLibPaths();

    m_ui.builtInCombo->addItems(detectedPaths);
    m_ui.builtInCombo->addItems(manualPaths);
}

void ImportComponentDialog::builtInRadioToggled(bool checked)
{
    if (checked)
        refreshPaths();
    else
        m_ui.builtInCombo->clear();
}

void ImportComponentDialog::accept()
{
    if (m_ui.builtInRadio->isChecked())
        m_importer.importFromPath(m_ui.builtInCombo->currentText());
    else if (m_ui.customRadio->isChecked())
        m_importer.importFromPath(m_ui.asn1sccPathChooser->path());

    QDialog::accept();
}

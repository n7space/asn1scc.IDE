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

#include "importcomponentwizard.h"

#include <coreplugin/icore.h>

#include "selectsourcepage.h"
#include "selectcomponentspage.h"
#include "summarypage.h"

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

ImportComponentWizard::ImportComponentWizard(QWidget *parent)
    : QWizard(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    connect(Core::ICore::instance(), &Core::ICore::coreAboutToClose, this, &QWidget::close);

    addPage(new SelectSourcePage(m_importer));
    addPage(new SelectComponentsPage(m_importer));
    addPage(new SummaryPage(m_importer));

    setOption(NoBackButtonOnStartPage);
}

void ImportComponentWizard::accept()
{
    m_importer.import();

    QDialog::accept();
}

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

#include "selectcomponentspage.h"

#include <QBoxLayout>

#include <QDebug>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

SelectComponentsPage::SelectComponentsPage(ComponentImporter &importer, QWidget *parent)
    : QWizardPage(parent)
    , m_model(new QFileSystemModel)
    , m_modulesView(new QTreeView(this))
    , m_importer(importer)

{
    setTitle(QLatin1String("Select components you wish to import"));

    m_modulesView->setModel(m_model);

    auto layout = new QBoxLayout(QBoxLayout::TopToBottom);
    layout->addWidget(m_modulesView);
    setLayout(layout);
}

SelectComponentsPage::~SelectComponentsPage()
{
    delete m_model;
}

void SelectComponentsPage::initializePage()
{
    m_model->setRootPath(m_importer.path());
    m_modulesView->setRootIndex(m_model->index(m_importer.path()));
}

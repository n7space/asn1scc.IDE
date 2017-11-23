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

#include <libraries/metadata/library.h>
#include <libraries/librarystorage.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

SelectComponentsPage::SelectComponentsPage(ComponentImporter &importer, QWidget *parent)
    : QWizardPage(parent)
    , m_modulesView(new QTreeView(this))
    , m_importer(importer)

{
    setTitle(QLatin1String("Select components you wish to import"));

    auto layout = new QBoxLayout(QBoxLayout::TopToBottom);
    layout->addWidget(m_modulesView);
    setLayout(layout);

    m_modulesView->setHeaderHidden(true);
    m_modulesView->setExpandsOnDoubleClick(false);

    connect(m_modulesView, &QTreeView::clicked,
            this, &SelectComponentsPage::onItemClicked);
}

void SelectComponentsPage::initializePage()
{
    auto storage = LibraryStorage::instance();
    const auto lib = storage->library(m_importer.path());

    if (lib == nullptr)
        return;

    m_model.reset(new LibraryModel(lib, this));
    m_modulesView->setModel(m_model.get());
}

void SelectComponentsPage::onItemClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    auto node = static_cast<Metadata::LibraryNode *>(index.internalPointer());
    if (!node)
        return;

    node->setChecked(!node->checked());

    m_modulesView->update(index);
}

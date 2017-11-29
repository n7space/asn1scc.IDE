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

#include <libraries/metadata/library.h>
#include <libraries/librarystorage.h>

#include "metadatacomponentselector.h"

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

static const QString METADATA_COMBO_KEY("Metadata");
static const QString FILESYSTEM_COMBO_KEY("File System");

SelectComponentsPage::SelectComponentsPage(ComponentImporter &importer, QWidget *parent)
    : QWizardPage(parent)
    , m_importer(importer)
{
    m_ui.setupUi(this);

    setTitle(QLatin1String("Select components you wish to import"));

    connect(m_ui.modulesView, &QTreeView::clicked,
            this, &SelectComponentsPage::onTreeItemClicked);

    connect(m_ui.modeCombo, &QComboBox::currentTextChanged,
            this, &SelectComponentsPage::onComboTextChanged);
}

void SelectComponentsPage::initializePage()
{
    m_ui.modeCombo->setCurrentIndex(0);

    setLibPath();
    m_ui.modeCombo->setEnabled(LibraryStorage::instance()->library(m_libPath) != nullptr);

    setupFileSystemModel();
}

bool SelectComponentsPage::validatePage()
{
    if (m_selector != nullptr)
        m_importer.setFiles(m_selector->paths());

    return QWizardPage::validatePage();
}

void SelectComponentsPage::onTreeItemClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    auto node = static_cast<Metadata::LibraryNode *>(index.internalPointer());
    if (!node)
        return;

    node->setChecked(!node->checked());

    m_ui.modulesView->update(index);
}

void SelectComponentsPage::onComboTextChanged(const QString &text)
{
    if (text == METADATA_COMBO_KEY)
        setupMetadaModel();
    else if (text == FILESYSTEM_COMBO_KEY)
        setupFileSystemModel();
}

void SelectComponentsPage::setLibPath()
{
    m_libPath = field("builtInRadio").toBool() ?
                field("builtInCombo").toString() :
                field("pathChoser").toString();
}

void SelectComponentsPage::setupMetadaModel()
{
    auto model = new LibraryModel(LibraryStorage::instance()->library(m_libPath), this);

    m_ui.modulesView->setModel(model);

    m_model.reset(model);
    m_selector.reset(new MetadaComponentSelector(m_libPath));
}

void SelectComponentsPage::setupFileSystemModel()
{
    // TODO: create filesystem model, setup it here
    // TODO: create filesystem selector, setup it here
    m_model.reset(nullptr);
    m_selector.reset(nullptr);

    m_ui.modulesView->setModel(m_model.get());
}

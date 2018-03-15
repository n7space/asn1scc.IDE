/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
** Contact: http://n7space.com
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

#include <utils/qtcassert.h>

#include <libraries/filemodel.h>
#include <libraries/librarystorage.h>
#include <libraries/metadata/library.h>

#include "filecomponentselector.h"
#include "metadatacomponentselector.h"

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

static const QString FILESYSTEM_COMBO_KEY("File System");
static const QString METADATA_COMBO_KEY("Metadata");

SelectComponentsPage::SelectComponentsPage(ComponentImporter &importer, QWidget *parent)
    : QWizardPage(parent)
    , m_importer(importer)
{
    m_ui.setupUi(this);

    setTitle(QLatin1String("Select components you wish to import"));

    connect(m_ui.modeCombo,
            &QComboBox::currentTextChanged,
            this,
            &SelectComponentsPage::onComboTextChanged);
}

void SelectComponentsPage::initializePage()
{
    setLibPath();

    const auto lib = LibraryStorage::instance()->library(m_libPath);
    lib == nullptr ? intializeForSelectedLib(lib, FILESYSTEM_COMBO_KEY)
                   : intializeForSelectedLib(lib, METADATA_COMBO_KEY);
}

bool SelectComponentsPage::validatePage()
{
    m_importer.setFiles(m_selector->pathsToImport());

    return QWizardPage::validatePage();
}

void SelectComponentsPage::onComboTextChanged(const QString &text)
{
    setupModel(text);
}

void SelectComponentsPage::setLibPath()
{
    m_libPath = field("builtInRadio").toBool() ? field("builtInCombo").toString()
                                               : field("pathChoser").toString();
}

void SelectComponentsPage::setupModel(const QString &key)
{
    QTC_ASSERT(key == METADATA_COMBO_KEY || key == FILESYSTEM_COMBO_KEY, return );

    if (key == METADATA_COMBO_KEY) {
        setupMetadaModel();
        m_ui.verticalWidget->show();
    } else if (key == FILESYSTEM_COMBO_KEY) {
        setupFileSystemModel();
        m_ui.verticalWidget->hide();
    }
}

void SelectComponentsPage::setupMetadaModel()
{
    auto model = new MetadataModel(LibraryStorage::instance()->library(m_libPath), this);

    m_ui.modulesView->setModel(model);

    m_selector.reset(new MetadaComponentSelector(model, m_libPath));
    m_labelsController.reset(
        new RelationsLabelsController(model, m_ui.requiresTree, m_ui.conflictsTree));

    connect(m_ui.modulesView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            m_labelsController.get(),
            &RelationsLabelsController::onSelectionChanged,
            Qt::UniqueConnection);

    m_model.reset(model);
}

void SelectComponentsPage::setupFileSystemModel()
{
    auto model = new FileModel(this);

    static QStringList nameFilter = (QStringList() << "*.asn1"
                                                   << "*.asn"
                                                   << "*.acn");
    model->setNameFilters(nameFilter);
    model->setNameFilterDisables(false);

    model->setRootPath(m_libPath);

    m_ui.modulesView->setModel(model);
    m_ui.modulesView->setRootIndex(model->index(m_libPath));

    m_selector.reset(new FileComponentSelector(model, nameFilter));
    m_labelsController.reset();

    m_model.reset(model);
}

void SelectComponentsPage::intializeForSelectedLib(const Metadata::Library *const lib,
                                                   const QString &key)
{
    const int idx = m_ui.modeCombo->findText(key);

    QTC_ASSERT(idx != -1, return );

    m_ui.modeCombo->setCurrentIndex(idx);
    m_ui.modeCombo->setEnabled(lib != nullptr);
    setupModel(key);
}

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

#pragma once

#include <memory>

#include <QTreeView>
#include <QWizardPage>

#include <libraries/componentimporter.h>
#include <libraries/metadatamodel.h>

#include "componentselector.h"
#include "relationslabelscontroller.h"

#include "ui_select_component.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {

namespace Metadata {
class Library;
}

namespace Wizard {

class SelectComponentsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SelectComponentsPage(ComponentImporter &importer, QWidget *parent = nullptr);

    void initializePage() override;
    bool validatePage() override;

private slots:
    void onComboTextChanged(const QString &text);

    void onLibrariesInvalidated();
    void onLibrariesValidated();

private:
    void setLibPath();

    void setupModel(const QString &key);
    void setupMetadaModel();
    void setupFileSystemModel();
    void intializeForSelectedLib(const Metadata::Library *const lib, const QString &key);

    Ui::SelectComponent m_ui;
    QString m_libPath;
    QString m_currentKey;

    std::unique_ptr<QAbstractItemModel> m_model;

    ComponentImporter &m_importer;
    std::unique_ptr<ComponentSelector> m_selector;
    std::unique_ptr<RelationsLabelsController> m_labelsController;
};

} // namespace Wizard
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

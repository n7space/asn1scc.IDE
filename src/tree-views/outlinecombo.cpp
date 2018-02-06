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
#include "outlinecombo.h"

#include "activatehandler.h"

#include "combomodel.h"
#include "outlineindexupdater.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;

static const int MINIMUM_COMBO_CONTENTS_LENGHT = 22;
static const int MAXIMUM_COMBO_VISIBLE_ITEMS = 40;

OutlineCombo::OutlineCombo(const QString &filePath, TextEditor::TextEditorWidget *textEditor)
{
    auto model = new ComboModel(filePath, this);

    auto indexUpdater = new OutlineIndexUpdater(model, this);
    indexUpdater->setEditor(textEditor);

    setupComboBox(model);

    connect(indexUpdater, &IndexUpdater::currentIndexUpdated,
            this, &OutlineCombo::updateSelection);

    connect(model, &ComboModel::modelReset,
            this, &OutlineCombo::modelRootChanged);

    view()->expandAll();
}

void OutlineCombo::setupComboBox(Model *model)
{
    setModel(model);
    setMinimumContentsLength(MINIMUM_COMBO_CONTENTS_LENGHT);

    auto policy = sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    setSizePolicy(policy);

    setMaxVisibleItems(MAXIMUM_COMBO_VISIBLE_ITEMS);
    setContextMenuPolicy(Qt::NoContextMenu);

    connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            [this]() { ActivateHandler::gotoSymbol(view()->currentIndex()); });
}

void OutlineCombo::modelRootChanged()
{
    view()->expandAll();
}

void OutlineCombo::updateSelection(const QModelIndex index)
{
    if (index.isValid())
        setCurrentIndex(index);
    else
        QComboBox::setCurrentIndex(0);
}

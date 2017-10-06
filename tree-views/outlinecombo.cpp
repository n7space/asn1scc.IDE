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
#include "outlinecombo.h"

#include <coreplugin/editormanager/editormanager.h>

#include "indexupdater.h"
#include "model.h"
#include "activatehandler.h"

using namespace Asn1Acn::Internal::TreeViews;

static const int MINIMUM_COMBO_CONTENTS_LENGHT = 22;
static const int MAXIMUM_COMBO_VISIBLE_ITEMS = 40;

OutlineCombo::OutlineCombo(Model *model, IndexUpdater *indexUpdater)
{
    // TODO indexUpdater
    setModel(model);
    setMinimumContentsLength(MINIMUM_COMBO_CONTENTS_LENGHT);

    auto policy = sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    setSizePolicy(policy);

    setMaxVisibleItems(MAXIMUM_COMBO_VISIBLE_ITEMS);
    setContextMenuPolicy(Qt::NoContextMenu);

    connect(model, &QAbstractItemModel::modelReset,
            view(), &Utils::TreeViewComboBoxView::expandAll);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
            view(), &Utils::TreeViewComboBoxView::expandAll);

    connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            [this]() { ActivateHandler::gotoSymbol(view()->currentIndex()); });

    connect(indexUpdater, &IndexUpdater::currentIndexUpdated,
            this, &OutlineCombo::updateSelection);
}

void OutlineCombo::updateSelection(const QModelIndex &index)
{
    if (index.isValid())
        setCurrentIndex(index);
    else
        QComboBox::setCurrentIndex(0);
}

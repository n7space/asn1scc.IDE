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

#include "structuresview.h"

#include <coreplugin/editormanager/editormanager.h>

#include "modeltree.h"
#include "asn1acnconstants.h"
#include "structuresviewindexupdater.h"

using namespace Asn1Acn::Internal;

StructuresViewWidget::StructuresViewWidget() :
    OverviewWidget(new OverviewModel)
{
    m_model->setRootNode(ModelTree::instance()->getModelTreeRoot());

    m_indexUpdater = createIndexUpdater();

    connect(m_model, &QAbstractItemModel::modelReset,
            this, &StructuresViewWidget::modelUpdated);
}

StructuresViewWidget::~StructuresViewWidget()
{
    delete m_model;
}

void StructuresViewWidget::modelUpdated()
{
    m_indexUpdater->updateCurrentIndex();
    m_treeView->expandAll();
}

std::shared_ptr<OverviewIndexUpdater> StructuresViewWidget::createIndexUpdater() const
{
    auto indexUpdater = std::make_shared<StructuresViewIndexUpdater>(m_model);

    connect(indexUpdater.get(), &OverviewIndexUpdater::currentIndexUpdated,
            this, &StructuresViewWidget::updateSelectionInTree);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
            indexUpdater.get(), &StructuresViewIndexUpdater::onEditorChanged);

    return indexUpdater;
}

StructuresViewFactory::StructuresViewFactory()
{
    setDisplayName(tr("Structures View"));
    setPriority(500);
    setId(Constants::STRUCTURES_VIEW_ID);
}

Core::NavigationView StructuresViewFactory::createWidget()
{
    return Core::NavigationView(new StructuresViewWidget);
}

/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

#include "asnstructuresview.h"

#include "asnparseddatastorage.h"
#include "asn1acnconstants.h"
#include "asnparsedobject.h"

using namespace Asn1Acn::Internal;

AsnStructuresViewWidget::AsnStructuresViewWidget() :
    DataStructuresWidget(new AsnOverviewModel),
    m_modelRoot(std::shared_ptr<AsnParsedObject>(new AsnParsedObject))
{
    connect(AsnParsedDataStorage::instance(), &AsnParsedDataStorage::storageUpdated,
            this, &AsnStructuresViewWidget::modelUpdated);

    modelUpdated();
}

AsnStructuresViewWidget::~AsnStructuresViewWidget()
{
    delete m_model;
}

void AsnStructuresViewWidget::refreshModel()
{
    m_modelRoot->detachChildren();

    AsnParsedDataStorage *instance = AsnParsedDataStorage::instance();
    auto documents = instance->getAllParsedFiles();
    for (const auto &document : documents)
        m_modelRoot->addChild(document->getTreeRoot());

    m_model->setRootNode(m_modelRoot);
}

void AsnStructuresViewWidget::modelUpdated()
{
    refreshModel();
    DataStructuresWidget::modelUpdated();
}

AsnStructuresViewFactory::AsnStructuresViewFactory()
{
    setDisplayName(tr("Structures View"));
    setPriority(500);
    setId(Constants::ASN_STRUCTURES_VIEW_ID);
}

Core::NavigationView AsnStructuresViewFactory::createWidget()
{
    return Core::NavigationView(new AsnStructuresViewWidget);
}

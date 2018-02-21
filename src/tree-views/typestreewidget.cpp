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
#include "typestreewidget.h"

#include <coreplugin/editormanager/editormanager.h>

#include <utils/utilsicons.h>

#include <asn1acnconstants.h>
#include <parseddatastorage.h>

#include "indexupdaterfactory.h"
#include "typestreeindexupdaterfactory.h"
#include "typestreemodel.h"

using namespace Asn1Acn::Internal::TreeViews;

TypesTreeWidget::TypesTreeWidget(Model *model, std::unique_ptr<IndexUpdaterFactory> factory)
    : TreeViewWidget(model, std::move(factory))
    , m_syncWithEditor(true)
{
    model->setRoot(ParsedDataStorage::instance()->root());

    m_toggleSync = createToggleSyncButton();
}

TypesTreeWidget::~TypesTreeWidget() {}

QToolButton *TypesTreeWidget::toggleSyncButton()
{
    return m_toggleSync;
}

void TypesTreeWidget::toggleCursorSynchronization()
{
    m_syncWithEditor = !m_syncWithEditor;
    setCursorSynchronization(m_syncWithEditor);
}

QToolButton *TypesTreeWidget::createToggleSyncButton()
{
    QToolButton *button = new QToolButton;

    button->setIcon(Utils::Icons::LINK.icon());
    button->setCheckable(true);
    button->setChecked(m_syncWithEditor);
    button->setToolTip(tr("Synchronize with Editor"));

    setCursorSynchronization(m_syncWithEditor);

    connect(button, &QAbstractButton::clicked, this, &TypesTreeWidget::toggleCursorSynchronization);

    return button;
}

TypesTreeWidgetFactory::TypesTreeWidgetFactory()
{
    setDisplayName(tr("ASN.1 Types View"));
    setPriority(500);
    setId(Constants::TYPESTREE_VIEW_ID);
}

Core::NavigationView TypesTreeWidgetFactory::createWidget()
{
    Core::NavigationView n;
    TypesTreeWidget *w = new TypesTreeWidget(new TypesTreeModel,
                                             std::make_unique<TypesTreeIndexUpdaterFactory>());

    n.widget = w;
    n.dockToolBarWidgets.append(w->toggleSyncButton());

    return n;
}

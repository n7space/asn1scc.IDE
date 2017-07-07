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

#include "editor.h"

#include <memory>

#include <QMenu>
#include <QString>

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>

#include "asn1acnconstants.h"
#include "document.h"
#include "modeltree.h"
#include "overviewmodel.h"

using namespace Asn1Acn::Internal;
using namespace Core;

EditorWidget::EditorWidget()
{
    // TODO ? setLanguageSettingsId(Constants::SettingsId);

    m_model = new OverviewModel(this);
}

EditorWidget::~EditorWidget()
{
    delete m_model;
}

void EditorWidget::finalizeInitialization()
{
    Document *doc = qobject_cast<Document *>(textDocument());
    connect(doc, &Document::documentUpdated,
            this, &EditorWidget::onAsnDocumentUpdated);
}

OverviewModel *EditorWidget::getOverviewModel() const
{
    return m_model;
}

void EditorWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QPointer<QMenu> menu(new QMenu(this));

    ActionContainer *mcontext = ActionManager::actionContainer(Constants::CONTEXT_MENU);
    QMenu *contextMenu = mcontext->menu();

    foreach (QAction *action, contextMenu->actions()) {
        menu->addAction(action);
    }

    appendStandardContextMenuActions(menu);

    menu->exec(e->globalPos());
    if (!menu)
        return;
    delete menu;
}

void EditorWidget::onAsnDocumentUpdated()
{
    QString filePath = textDocument()->filePath().toString();
    ModelTree *tree = ModelTree::instance();

    ModelTreeNode::ModelTreeNodePtr documentNode = tree->getAnyNodeForFilepath(filePath);
    if (documentNode == nullptr)
        return;

    m_model->setRootNode(documentNode);
}

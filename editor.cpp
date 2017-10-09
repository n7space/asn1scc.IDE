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

#include "editor.h"

#include <QMenu>
#include <QString>

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>

#include <tree-views/outlinecombo.h>

#include "asn1acnconstants.h"
#include "document.h"

#include "editoroutline.h"

using namespace Asn1Acn::Internal;
using namespace Core;

EditorWidget::EditorWidget()
    : m_editorOutline(new EditorOutline(this))
{
}

EditorOutline *EditorWidget::outline() const
{
    return m_editorOutline;
}

void EditorWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QPointer<QMenu> menu(new QMenu(this));

    ActionContainer *mcontext = ActionManager::actionContainer(Constants::CONTEXT_MENU);
    QMenu *contextMenu = mcontext->menu();

    foreach (QAction *action, contextMenu->actions())
        menu->addAction(action);

    appendStandardContextMenuActions(menu);

    menu->exec(e->globalPos());
    if (!menu)
        return;

    delete menu;
}

void EditorWidget::finalizeInitialization()
{
    // TODO finalizeInitializationAfterDuplication
    // TODO ? setLanguageSettingsId(Constants::SettingsId);

    auto document = qobject_cast<Document *>(textDocument());

    connect(document, &Document::extraSelectionsUpdated,
            [this](const QList<QTextEdit::ExtraSelection> &selections){
        setExtraSelections(TextEditorWidget::CodeWarningsSelection, selections);
    });

    insertExtraToolBarWidget(TextEditorWidget::Left,
                             new TreeViews::OutlineCombo(m_editorOutline->model(), m_editorOutline->indexUpdater()));
}

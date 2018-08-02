/****************************************************************************
**
** Copyright (C) 2017-2018 N7 Space sp. z o. o.
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

#include "editor.h"

#include <QMenu>
#include <QString>
#include <QTextCursor>

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>

#include <tree-views/outlinecombo.h>

#include "asn1acnconstants.h"
#include "document.h"
#include "selectionpositionresolver.h"

using namespace Asn1Acn::Internal;
using namespace Core;

void EditorWidget::openFinishedSuccessfully()
{
    addOutlineCombo();

    TextEditorWidget::openFinishedSuccessfully();
}

void EditorWidget::finalizeInitializationAfterDuplication(TextEditorWidget *other)
{
    Q_UNUSED(other);

    addOutlineCombo();
}

void EditorWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTextCursor cursor = textCursor();

    const int initPos = cursor.position();
    TextEditorWidget::mouseDoubleClickEvent(e);
    cursor.setPosition(initPos);

    static SelectionPositionResolver resolver;
    resolver.resolve(cursor);

    cursor.setPosition(resolver.startPosition());
    cursor.setPosition(resolver.endPosition(), QTextCursor::KeepAnchor);

    setTextCursor(cursor);
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
    // TODO ? setLanguageSettingsId(Constants::SettingsId);

    auto document = qobject_cast<Document *>(textDocument());
    connect(document,
            &Document::extraSelectionsUpdated,
            this,
            &EditorWidget::onExtraSelectionsUpdated);
}

void EditorWidget::onExtraSelectionsUpdated(const QList<QTextEdit::ExtraSelection> &selections)
{
    setExtraSelections(TextEditorWidget::CodeWarningsSelection, selections);
}

void EditorWidget::addOutlineCombo()
{
    insertExtraToolBarWidget(TextEditorWidget::Left,
                             new TreeViews::OutlineCombo(textDocument()->filePath(), this));
}

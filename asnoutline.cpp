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

#include "asnoutline.h"

#include <texteditor/textdocument.h>

#include <coreplugin/find/itemviewfind.h>
#include <coreplugin/editormanager/editormanager.h>
#include <utils/qtcassert.h>

#include <QVBoxLayout>
#include <QMenu>

namespace Asn1Acn {
namespace Internal {

AsnOutlineTreeView::AsnOutlineTreeView(QWidget *parent) :
    Utils::NavigationTreeView(parent)
{
}

void AsnOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    // TODO: no need to handle events until the data is stubbed
    /* if (!event)
        return;

    QMenu contextMenu;

    QAction *action = contextMenu.addAction(tr("Expand All"));
    connect(action, &QAction::triggered, this, &QTreeView::expandAll);
    action = contextMenu.addAction(tr("Collapse All"));
    connect(action, &QAction::triggered, this, &QTreeView::collapseAll);

    contextMenu.exec(event->globalPos());

    event->accept(); */
}

void AsnOutlineWidget::modelUpdated()
{
    m_treeView->expandAll();
}

AsnOutlineWidget::AsnOutlineWidget(AsnEditorWidget *editor) :
    TextEditor::IOutlineWidget(),
    m_editor(editor),
    m_treeView(new AsnOutlineTreeView(this)),
    m_model(m_editor->getOverviewModel())
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(Core::ItemViewFind::createSearchableWrapper(m_treeView));
    setLayout(layout);

    m_treeView->setModel(m_model);

    connect(m_model, &QAbstractItemModel::modelReset, this, &AsnOutlineWidget::modelUpdated);
    modelUpdated();
}

QList<QAction *> AsnOutlineWidget::filterMenuActions() const
{
    return QList<QAction *>();
}

void AsnOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
    Q_UNUSED(syncWithCursor);
}

bool AsnOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
    return qobject_cast<AsnEditor *>(editor) != nullptr;
}

TextEditor::IOutlineWidget *AsnOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
    AsnEditor *asnEditor = qobject_cast<AsnEditor *>(editor);
    QTC_ASSERT(asnEditor, return 0);

    AsnEditorWidget *asnEditorWidget = qobject_cast<AsnEditorWidget *>(asnEditor->widget());
    QTC_ASSERT(asnEditorWidget, return 0);

    AsnOutlineWidget *widget = new AsnOutlineWidget(asnEditorWidget);
    return widget;
}

} // namespace Internal
} // namespace Asn1Acn

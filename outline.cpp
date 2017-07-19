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

#include "outline.h"

#include <QAbstractItemModel>

#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>

#include <utils/fileutils.h>
#include <utils/qtcassert.h>

#include "asneditor.h"
#include "acneditor.h"

#include "modeltree.h"

using namespace Asn1Acn::Internal;

OutlineWidget::OutlineWidget(EditorWidget *editor) :
    OverviewWidget(editor->getOverviewModel()),
    m_editor(editor)
{
    connect(m_model, &QAbstractItemModel::modelReset,
            this, &OutlineWidget::modelUpdated);

    if (ModelTree::instance()->isValid())
        modelUpdated();
}

void OutlineWidget::modelUpdated()
{
    const QString &path = m_editor->textDocument()->filePath().toString();

    ModelTreeNode::ModelTreeNodePtr documentNode = ModelTree::instance()->getAnyNodeForFilepath(path);
    m_model->setRootNode(documentNode);

    m_treeView->expandAll();
}

bool OutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
    return qobject_cast<AcnEditor *>(editor) != nullptr
            || qobject_cast<AsnEditor *>(editor) != nullptr;
}

TextEditor::IOutlineWidget *OutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
    TextEditor::BaseTextEditor *baseEditor = qobject_cast<TextEditor::BaseTextEditor *>(editor);
    QTC_ASSERT(baseEditor, return 0);

    EditorWidget *editorWidget = qobject_cast<EditorWidget *>(baseEditor->widget());
    QTC_ASSERT(editorWidget, return 0);

    OutlineWidget *widget = new OutlineWidget(editorWidget);
    return widget;
}

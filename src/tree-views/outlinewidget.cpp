/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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
#include "outlinewidget.h"

#include <memory>

#include <utils/qtcassert.h>

#include <acneditor.h>
#include <asneditor.h>
#include <editor.h>

#include <texteditor/textdocument.h>

#include <tree-views/outlineindexupdaterfactory.h>
#include <tree-views/outlinemodel.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;

OutlineWidget::OutlineWidget(Model *model, std::unique_ptr<OutlineIndexUpdaterFactory> factory)
    : TreeViewWidget(model, std::move(factory))
{
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

    return new OutlineWidget(new OutlineModel(editorWidget->textDocument()->filePath()),
                             std::make_unique<OutlineIndexUpdaterFactory>());
}

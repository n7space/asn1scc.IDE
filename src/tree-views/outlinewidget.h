/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
** Contact: https://n7space.com
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
#pragma once

#include <texteditor/ioutlinewidget.h>

#include "outlineindexupdaterfactory.h"
#include "treeviewwidget.h"

namespace Asn1Acn {
namespace Internal {

class EditorWidget;

namespace TreeViews {

class OutlineWidget : public TreeViewWidget
{
    Q_OBJECT
public:
    OutlineWidget(Model *model, std::unique_ptr<OutlineIndexUpdaterFactory> factory);
};

class OutlineWidgetFactory : public TextEditor::IOutlineWidgetFactory
{
    Q_OBJECT
public:
    bool supportsEditor(Core::IEditor *editor) const override;
    TextEditor::IOutlineWidget *createWidget(Core::IEditor *editor) override;
};

} // namespace TreeViews
} // namespace Internal
} // namespace Asn1Acn

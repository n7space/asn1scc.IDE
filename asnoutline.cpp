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

#include <QAbstractItemModel>

#include <utils/qtcassert.h>

using namespace Asn1Acn::Internal;

AsnOutlineWidget::AsnOutlineWidget(AsnEditorWidget *editor) :
    DataStructuresWidget(editor->getOverviewModel()),
    m_editor(editor)
{
    connect(m_model, &QAbstractItemModel::modelReset,
            this, &AsnOutlineWidget::modelUpdated);

    modelUpdated();
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

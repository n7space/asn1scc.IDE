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
#include "activatehandler.h"

#include <coreplugin/editormanager/editormanager.h>

#include "model.h"

using namespace Asn1Acn::Internal::TreeViews;

void ActivateHandler::gotoSymbol(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    const auto model = qobject_cast<const Model *>(index.model());
    if (!model)
        return;

    const auto node = model->dataNode(index);
    if (!node)
        return;

    const auto location = node->location();
    Core::EditorManager::openEditorAt(location.path(), location.line(), location.column());
}

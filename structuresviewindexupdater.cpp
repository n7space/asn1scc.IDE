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

#include "structuresviewindexupdater.h"

#include <QAbstractItemModel>

#include <texteditor/texteditor.h>
#include <texteditor/textdocument.h>

#include <utils/fileutils.h>

using namespace Asn1Acn::Internal;

StructuresViewIndexUpdater::StructuresViewIndexUpdater(const OverviewModel *model)
    : OverviewIndexUpdater(model)
{
}

QModelIndex StructuresViewIndexUpdater::getCurrentFileIndex() const
{
    return getIndexFromPath(m_editorWidget->textDocument()->filePath().toString());
}

QModelIndex StructuresViewIndexUpdater::getIndexFromPath(const QString &path) const
{
    QModelIndex rootIndex = m_model->index(0, 0, QModelIndex());

    for (int row = 0; row < m_model->rowCount(rootIndex); ++row) {
        const QModelIndex index = m_model->index(row, 0, rootIndex);
        const ModelTreeNode *symbol = static_cast<ModelTreeNode *>(index.internalPointer());
        const Data::SourceLocation location = symbol->getSourceLocation();

        if (location.path() == path)
            return index;
    }

    return QModelIndex();
}

void StructuresViewIndexUpdater::onEditorChanged(Core::IEditor *editor)
{
    Q_UNUSED(editor);

    TextEditor::TextEditorWidget *editorWidget = TextEditor::TextEditorWidget::currentTextEditorWidget();
    setEditor(editorWidget);
}

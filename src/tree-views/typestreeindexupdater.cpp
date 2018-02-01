/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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
#include "typestreeindexupdater.h"

#include <projectexplorer/session.h>
#include <projectexplorer/project.h>

#include <coreplugin/editormanager/editormanager.h>

#include <parseddatastorage.h>
#include <data/root.h>
#include <data/project.h>

#include "typestree-visitors/indexfindingvisitor.h"

#include "model.h"

using namespace Asn1Acn::Internal::TreeViews;

TypesTreeIndexUpdater::TypesTreeIndexUpdater(const Model *model, QObject *parent)
    : IndexUpdater(model, parent)
{
    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
            this, &TypesTreeIndexUpdater::onEditorChanged);

    connect(Core::EditorManager::instance(), &Core::EditorManager::editorAboutToClose,
            this, &TypesTreeIndexUpdater::onEditorAboutToClose);

    connect(m_model, &Model::modelReset,
            [this]() { updateCurrentIndex(); } );
}

QModelIndex TypesTreeIndexUpdater::currentRootIndex() const
{
    const auto &root = ParsedDataStorage::instance()->root();
    const auto project = ProjectExplorer::SessionManager::projectForFile(currentFileName());
    if (!project)
        return QModelIndex();
    const auto projectNode = root->project(project->displayName());
    if (!projectNode)
        return QModelIndex();
    return m_model->index(root->valueFor<TypesTreeVisitors::IndexFindingVisitor>(projectNode), 0, QModelIndex());
}

void TypesTreeIndexUpdater::onEditorChanged(Core::IEditor *editor)
{
    Q_UNUSED(editor);
    TextEditor::TextEditorWidget *editorWidget = TextEditor::TextEditorWidget::currentTextEditorWidget();
    setEditor(editorWidget);
}

void TypesTreeIndexUpdater::onEditorAboutToClose(Core::IEditor *editor)
{
    Q_UNUSED(editor);
    unsetEditor();
}

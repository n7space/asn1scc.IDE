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
#include "editoroutline.h"

#include <texteditor/textdocument.h>
#include <utils/fileutils.h>
#include <coreplugin/editormanager/editormanager.h>

#include <tree-views/outlinemodel.h>
#include <tree-views/outlineindexupdater.h>

#include <editor.h>
#include <parseddatastorage.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;

static const int MINIMUM_COMBO_CONTENTS_LENGHT = 22;
static const int MAXIMUM_COMBO_VISIBLE_ITEMS = 40;

EditorOutline::EditorOutline(EditorWidget *editorWidget)
    : QObject(editorWidget)
    , m_editorWidget(editorWidget)
    , m_model(new OutlineModel(this))
    , m_indexUpdater(new OutlineIndexUpdater(m_model, this))
{
    m_indexUpdater->setEditor(m_editorWidget);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
            this, &EditorOutline::onEditorChanged);//view(), &Utils::TreeViewComboBoxView::expandAll);

    connect(ParsedDataStorage::instance(), &ParsedDataStorage::fileUpdated,
            [this](const QString &filePath, std::shared_ptr<Data::File> newFile) {
        if (m_editorWidget->textDocument()->filePath().toString() == filePath)
            m_model->setRoot(newFile.get());
    });
}

void EditorOutline::onEditorChanged()
{
    const QString &path = m_editorWidget->textDocument()->filePath().toString();

    const auto file = ParsedDataStorage::instance()->getFileForPath(path);

    m_model->setRoot(file.get());
}

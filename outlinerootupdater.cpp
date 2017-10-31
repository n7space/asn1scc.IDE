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

#include "outlinerootupdater.h"

#include <QString>

#include <texteditor/textdocument.h>
#include <coreplugin/editormanager/editormanager.h>

#include <tree-views/model.h>
#include <tree-views/outlineindexupdater.h>

#include <editor.h>

#include <parseddatastorage.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;

OutlineRootUpdater::OutlineRootUpdater(EditorWidget *editorWidget,
                                       Model *model,
                                       IndexUpdater *updater,
                                       QObject *parent)
    : QObject(parent)
    , m_editorWidget(editorWidget)
    , m_model(model)
    , m_indexUpdater(updater)
{
    m_indexUpdater->setEditor(m_editorWidget);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
            this, &OutlineRootUpdater::onEditorChanged);

    connect(m_model, &Model::modelReset,
            this, &OutlineRootUpdater::onModelReset);
}

void OutlineRootUpdater::onEditorChanged()
{
    refreshModelRoot();
    emit rootChanged();
}

void OutlineRootUpdater::onModelReset()
{
    refreshModelRoot();
    m_indexUpdater->updateCurrentIndex();
    emit rootChanged();
}

void OutlineRootUpdater::refreshModelRoot()
{
    const QString &path = m_editorWidget->textDocument()->filePath().toString();

    const auto file = ParsedDataStorage::instance()->getAnyFileForPath(path);

    m_model->setRoot(file);
}

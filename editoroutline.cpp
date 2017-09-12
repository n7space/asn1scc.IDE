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

#include <coreplugin/editormanager/editormanager.h>

#include "utils/qtcassert.h"

#include "editor.h"
#include "modeltree.h"

#include "overviewactivatehandler.h"

#include <QDebug>

using namespace Asn1Acn::Internal;

static const int MINIMUM_COMBO_CONTENTS_LENGHT = 22;
static const int MAXIMUM_COMBO_VISIBLE_ITEMS = 40;

EditorOutline::EditorOutline(TextEditor::TextEditorWidget *textEditorWidget)
    : QObject(textEditorWidget)
    , m_model(new OverviewModel(this))
    , m_indexUpdater(std::make_shared<OverviewIndexUpdater>(m_model))
    , m_combo(new Utils::TreeViewComboBox)
{
    setupCombo();

    m_editorWidget = qobject_cast<EditorWidget *>(textEditorWidget);
    QTC_ASSERT(m_editorWidget, return);

    m_indexUpdater->setEditor(m_editorWidget);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
            this, &EditorOutline::onEditorChanged);

    connect(m_indexUpdater.get(), &OverviewIndexUpdater::currentIndexUpdated,
            this, &EditorOutline::updateSelectionInTree);

    connect(m_model, &QAbstractItemModel::modelReset,
            this, &EditorOutline::modelUpdated);
}

QWidget *EditorOutline::takeWidget() const
{
    return m_combo;
}

OverviewModel *EditorOutline::model() const
{
    return m_model;
}

const std::shared_ptr<OverviewIndexUpdater> &EditorOutline::indexUpdater() const
{
    return m_indexUpdater;
}

void EditorOutline::itemActivated()
{
    OverviewActivateHandler::gotoSymbol(m_combo->view()->currentIndex());
}

void EditorOutline::updateSelectionInTree(const QModelIndex &index)
{
    if (index.isValid())
        m_combo->setCurrentIndex(index);
    else
        m_combo->QComboBox::setCurrentIndex(0);
}

void EditorOutline::onEditorChanged()
{
    m_combo->view()->expandAll();
}

void EditorOutline::modelUpdated()
{
    m_combo->view()->expandAll();
    m_indexUpdater->updateCurrentIndex();
}

void EditorOutline::setupCombo()
{
    m_combo->setModel(m_model);
    m_combo->setMinimumContentsLength(MINIMUM_COMBO_CONTENTS_LENGHT);

    auto policy = m_combo->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    m_combo->setSizePolicy(policy);

    m_combo->setMaxVisibleItems(MAXIMUM_COMBO_VISIBLE_ITEMS);
    m_combo->setContextMenuPolicy(Qt::NoContextMenu);

    connect(m_combo, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this, &EditorOutline::itemActivated);
}

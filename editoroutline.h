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

#pragma once

#include <QObject>

#include <texteditor/texteditor.h>
#include <utils/treeviewcombobox.h>

#include "overviewmodel.h"
#include "overviewindexupdater.h"

namespace Asn1Acn {
namespace Internal {

class EditorWidget;

class EditorOutline : public QObject
{
    Q_OBJECT
public:
    EditorOutline(TextEditor::TextEditorWidget *editorWidget);

    QWidget *takeWidget() const;

    OverviewModel *model() const;
    const std::shared_ptr<OverviewIndexUpdater> &indexUpdater() const;

public slots:
    void modelUpdated();

private slots:
    void updateSelectionInTree(const QModelIndex &index);
    void itemActivated();
    void onEditorChanged();

private:
    void setupCombo();

    EditorWidget *m_editorWidget;

    OverviewModel *m_model;
    std::shared_ptr<OverviewIndexUpdater> m_indexUpdater;

    Utils::TreeViewComboBox *m_combo;
};

} /* namespace Asn1Acn */
} /* namespace Internal */

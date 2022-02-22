/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
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

#include <QModelIndex>
#include <QObject>
#include <QTimer>

#include <texteditor/texteditor.h>
#include <utils/fileutils.h>

#include <data/sourcelocation.h>

#include "indexupdater.h"

class QTreeView;

namespace Asn1Acn {
namespace Internal {

class ParsedDataStorage;

namespace TreeViews {

class SynchronizedIndexUpdater : public IndexUpdater
{
    Q_OBJECT
public:
    explicit SynchronizedIndexUpdater(const Model *model,
                                      const ParsedDataStorage *const storage,
                                      QObject *parent);

    void updateCurrentIndex() override;

    void setEditor(TextEditor::TextEditorWidget *editorWidget);
    void unsetEditor();

protected:
    QModelIndex findIndexInLocation(const QModelIndex &parentIndex,
                                    Data::SourceLocation cursorLocation) const;
    Data::SourceLocation getCurrentLocation() const;
    Utils::FilePath currentFilePath() const;
    bool editorEmpty() const;

    const ParsedDataStorage *const m_storage;

private:
    virtual void fillSelectedIndexes(QModelIndex &current, QModelIndexList &selected) const = 0;

    void updateNow();
    bool updateAllowed();
    void createUpdateTimer();

    TextEditor::TextEditorWidget *m_editorWidget;
    QTimer *m_updateIndexTimer;
};

} /* namespace TreeViews */
} // namespace Internal
} // namespace Asn1Acn

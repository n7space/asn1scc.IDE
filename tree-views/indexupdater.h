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
#include <QTimer>
#include <QModelIndex>

#include <texteditor/texteditor.h>
/*#include <QAbstractItemModel>

#include "coreplugin/editormanager/ieditor.h"
*/
//#include "model.h"

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {

class Model;

class IndexUpdater : public QObject
{
    Q_OBJECT
protected:
    explicit IndexUpdater(const Model *model);

public:
    virtual ~IndexUpdater() = default;

    void setEditor(TextEditor::TextEditorWidget *editorWidget);
    void updateCurrentIndex();

signals:
    void currentIndexUpdated(const QModelIndex &modelIndex);

protected:
    virtual QModelIndex getCurrentFileIndex() const = 0;

private:
    void updateNow();
    void createUpdateTimer();

    int getCurrentLine() const;
    QModelIndex getTargetIndexFromFileIndex(const QModelIndex &fileIndex) const;
    QModelIndex getTargetIndexFromModuleIndex(const QModelIndex &moduleIndex, int line) const;

    const Model *m_model;
    TextEditor::TextEditorWidget *m_editorWidget;
    QTimer *m_updateIndexTimer;
};

} /* namespace TreeViews */
} /* namespace Asn1Acn */
} /* namespace Internal */

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

#pragma once

#include "asneditor.h"

#include <texteditor/ioutlinewidget.h>

#include <utils/navigationtreeview.h>

namespace Asn1Acn {
namespace Internal {

class AsnOutlineItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AsnOutlineItemModel(QObject *parent = 0);
    ~AsnOutlineItemModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row,
                      int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void addEntry(const QString &entry);
    void flushEntries(void);

private:
    QVariant *m_rootItem;
    QList<QVariant *> m_itemsData;
};

class AsnOutlineTreeView : public Utils::NavigationTreeView
{
    Q_OBJECT
public:
    AsnOutlineTreeView(QWidget *parent);

    void contextMenuEvent(QContextMenuEvent *event) override;
};

class AsnOutlineWidget : public TextEditor::IOutlineWidget
{
    Q_OBJECT
public:
    AsnOutlineWidget(AsnEditorWidget *editor);

    QList<QAction *> filterMenuActions() const override;
    void setCursorSynchronization(bool syncWithCursor) override;

private:
    void updateModel();

private:
    AsnEditorWidget *m_editor;
    AsnOutlineTreeView *m_treeView;
    AsnOutlineItemModel *m_model;
};

class AsnOutlineWidgetFactory : public TextEditor::IOutlineWidgetFactory
{
    Q_OBJECT
public:
    bool supportsEditor(Core::IEditor *editor) const override;
    TextEditor::IOutlineWidget *createWidget(Core::IEditor *editor) override;
};

} // namespace Internal
} // namespace Asn1Acn

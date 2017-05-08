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

#include "asnoutline.h"

#include <texteditor/textdocument.h>

#include <coreplugin/find/itemviewfind.h>
#include <coreplugin/editormanager/editormanager.h>
#include <utils/qtcassert.h>

#include <QVBoxLayout>
#include <QMenu>

namespace Asn1Acn {
namespace Internal {

AsnOutlineItemModel::AsnOutlineItemModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_rootItem(new QVariant())
{
}

AsnOutlineItemModel::~AsnOutlineItemModel()
{
    qDeleteAll(m_itemsData);
    delete m_rootItem;
}

int AsnOutlineItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant AsnOutlineItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    QVariant *item = static_cast<QVariant *>(index.internalPointer());

    return *item;
}

Qt::ItemFlags AsnOutlineItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QVariant AsnOutlineItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    return *m_rootItem;
}

QModelIndex AsnOutlineItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid()) {
        QVariant *childItem = m_itemsData.value(row);
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex AsnOutlineItemModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int AsnOutlineItemModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_itemsData.count();

    return 0;
}

void AsnOutlineItemModel::addEntry(const QString &entry)
{
    beginInsertRows(QModelIndex(), m_itemsData.count(), m_itemsData.count() + 1);
    QVariant *item = new QVariant(entry);
    m_itemsData.append(item);
    endInsertRows();
}

void AsnOutlineItemModel::flushEntries()
{
    beginResetModel();
    qDeleteAll(m_itemsData);
    m_itemsData.clear();
    endResetModel();
}

AsnOutlineTreeView::AsnOutlineTreeView(QWidget *parent) :
    Utils::NavigationTreeView(parent)
{
}

void AsnOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    // TODO: no need to handle events until the data is stubbed
    /* if (!event)
        return;

    QMenu contextMenu;

    QAction *action = contextMenu.addAction(tr("Expand All"));
    connect(action, &QAction::triggered, this, &QTreeView::expandAll);
    action = contextMenu.addAction(tr("Collapse All"));
    connect(action, &QAction::triggered, this, &QTreeView::collapseAll);

    contextMenu.exec(event->globalPos());

    event->accept(); */
}

AsnOutlineWidget::AsnOutlineWidget(AsnEditorWidget *editor) :
    TextEditor::IOutlineWidget(),
    m_editor(editor),
    m_treeView(new AsnOutlineTreeView(this)),
    m_model(new AsnOutlineItemModel(this))
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(Core::ItemViewFind::createSearchableWrapper(m_treeView));
    setLayout(layout);

    m_treeView->setModel(m_model);
    updateModel();
}

QList<QAction *> AsnOutlineWidget::filterMenuActions() const
{
    return QList<QAction *>();
}

void AsnOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
    Q_UNUSED(syncWithCursor);
}

void AsnOutlineWidget::updateModel()
{
    m_model->flushEntries();

    TextEditor::TextDocument *currentDoc = m_editor->textDocument();
    const Utils::FileName fileName = currentDoc->filePath();
    m_model->addEntry(fileName.fileName());
}

bool AsnOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
    return qobject_cast<AsnEditor *>(editor) != nullptr;
}

TextEditor::IOutlineWidget *AsnOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
    AsnEditor *asnEditor = qobject_cast<AsnEditor *>(editor);
    QTC_ASSERT(asnEditor, return 0);

    AsnEditorWidget *asnEditorWidget = qobject_cast<AsnEditorWidget *>(asnEditor->widget());
    QTC_ASSERT(asnEditorWidget, return 0);

    AsnOutlineWidget *widget = new AsnOutlineWidget(asnEditorWidget);
    return widget;
}

} // namespace Internal
} // namespace Asn1Acn

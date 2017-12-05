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

#include "filecomponentselector.h"

#include <QDirIterator>

#include <utils/qtcassert.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

FileComponentSelector::FileComponentSelector(QTreeView *treeView, FileModel *model, const QStringList nameFilter)
    : ComponentSelector(treeView)
    , m_model(model)
    , m_nameFilter(nameFilter)
{
    connect(m_model, &QFileSystemModel::directoryLoaded,
            this, &FileComponentSelector::onDirectoryLoaded);
}

QStringList FileComponentSelector::pathsToImport()
{
    return selectedPathsFromModelItem(m_model->rootPath());
}

void FileComponentSelector::updateSelections(const QModelIndex &index)
{
    updateChildrenSelections(index, index.data(Qt::CheckStateRole));
    updateParentsSelection(index, parentState(index));
}

QVariant FileComponentSelector::parentState(const QModelIndex &index) const
{
    const auto checkState = index.data(Qt::CheckStateRole);
    const auto parent = index.parent();

    for (int i = 0; i < m_model->rowCount(parent); ++i)
        for (int j = 0; j < m_model->columnCount(parent); ++j)
            if (parent.child(i, j).data(Qt::CheckStateRole) != checkState)
                return Qt::PartiallyChecked;

    return checkState;
}

void FileComponentSelector::updateChildrenSelections(const QModelIndex &index, const QVariant checkState)
{
    if (!index.isValid())
        return;

    for (int i = 0; i < m_model->rowCount(index); ++i) {
        for (int j = 0; j < m_model->columnCount(index); ++j) {
            auto child = index.child(i, j);

            m_model->changeCheckState(child, checkState);
            m_treeView->dataChanged(child, child);
            updateChildrenSelections(child, checkState);
        }
    }
}

void FileComponentSelector::updateParentsSelection(const QModelIndex &index, const QVariant checkState)
{
    for (auto parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        m_model->changeCheckState(parent, checkState);
        m_treeView->dataChanged(parent, parent);
    }
}

void FileComponentSelector::onDirectoryLoaded(const QString &path)
{
    auto index = m_model->index(path);
    auto checkState = index.data(Qt::CheckStateRole);

    if (checkState == Qt::Unchecked)
        return;

    updateChildrenSelections(index, checkState);
}

QStringList FileComponentSelector::selectedPathsFromModelItem(const QString &parentPath) const
{
    const auto parent = m_model->index(parentPath);

    if (!parent.isValid())
        return {};

    QStringList paths;
    for (int i = 0; i < m_model->rowCount(parent); ++i)
        for (int j = 0; j < m_model->columnCount(parent); ++j)
            paths.append(pathsFromChildIndex(parent.child(i, j)));

    return paths;
}

QStringList FileComponentSelector::pathsFromChildIndex(const QModelIndex &child) const
{
    const auto state = child.data(Qt::CheckStateRole);
    if (state == Qt::Unchecked)
        return {};

    if (m_model->isDir(child))
        return m_model->rowCount(child) > 0
                ? selectedPathsFromModelItem(m_model->filePath(child))
                : selectedPathsFromFilesystemItem(m_model->filePath(child));

    QTC_ASSERT(state == Qt::Checked, return {});

    return { m_model->filePath(child) };
}

QStringList FileComponentSelector::selectedPathsFromFilesystemItem(const QString &path) const
{
    QDirIterator it(path, m_nameFilter, QDir::Files, QDirIterator::Subdirectories);

    QStringList paths;
    while (it.hasNext())
        paths.append(it.next());

    return paths;
}

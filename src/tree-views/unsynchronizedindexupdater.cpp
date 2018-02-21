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

#include "unsynchronizedindexupdater.h"

#include <QItemSelectionModel>
#include <QTreeView>

#include "model.h"

using namespace Asn1Acn::Internal::TreeViews;

UnsynchronizedIndexUpdater::UnsynchronizedIndexUpdater(const QTreeView *view,
                                                       const Model *model,
                                                       QObject *parent)
    : IndexUpdater(model, parent)
    , m_view(view)
{
    connect(model,
            &Model::modelAboutToBeReset,
            this,
            &UnsynchronizedIndexUpdater::onModelAboutToBeReset);
}

void UnsynchronizedIndexUpdater::onModelAboutToBeReset()
{
    clearSavedPaths();

    saveCurrentPath();
    saveSelectedPaths();
}

void UnsynchronizedIndexUpdater::updateCurrentIndex()
{
    const auto selected = restoreSelectedPaths();
    const auto current = restoreCurrentPath();

    emit indexSelectionUpdated(selected, current);
}

void UnsynchronizedIndexUpdater::clearSavedPaths()
{
    m_lastCurrentPath.clear();
    m_selectedTreePaths.clear();
}

void UnsynchronizedIndexUpdater::saveCurrentPath()
{
    m_lastCurrentPath = treePathForIndex(m_view->currentIndex());
}

void UnsynchronizedIndexUpdater::saveSelectedPaths()
{
    const auto selection = m_view->selectionModel()->selectedIndexes();

    for (const auto &index : selection) {
        const auto path = treePathForIndex(index);
        if (!path.empty())
            m_selectedTreePaths.append(path);
    }
}

QModelIndex UnsynchronizedIndexUpdater::restoreCurrentPath()
{
    return indexForTreePath(m_lastCurrentPath);
}

QModelIndexList UnsynchronizedIndexUpdater::restoreSelectedPaths()
{
    QModelIndexList selected;
    for (const auto &path : m_selectedTreePaths) {
        const auto index = indexForTreePath(path);
        if (index.isValid())
            selected.append(index);
    }

    return selected;
}

QStringList UnsynchronizedIndexUpdater::treePathForIndex(const QModelIndex &index) const
{
    QStringList path;
    for (auto i = index; i.isValid(); i = i.parent())
        path.push_front(i.data(Qt::DisplayRole).toString());

    return path;
}

QModelIndex UnsynchronizedIndexUpdater::indexForTreePath(const QStringList &path) const
{
    QModelIndex index;
    for (const auto &item : path) {
        index = findInParent(index, item);
        if (!index.isValid())
            return QModelIndex();
    }

    return index;
}

QModelIndex UnsynchronizedIndexUpdater::findInParent(const QModelIndex &parent,
                                                     const QString &displayName) const
{
    for (int i = 0; i < m_model->rowCount(parent); ++i)
        for (int j = 0; j < m_model->columnCount(parent); ++j)
            if (m_model->index(i, j, parent).data(Qt::DisplayRole).toString() == displayName)
                return m_model->index(i, j, parent);

    return QModelIndex();
}

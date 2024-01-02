/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
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

#include "indexupdater.h"

class QTreeView;

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {

class UnsynchronizedIndexUpdater : public IndexUpdater
{
    Q_OBJECT
public:
    explicit UnsynchronizedIndexUpdater(const QTreeView *view, const Model *model, QObject *parent);
    void updateCurrentIndex() override;

protected slots:
    void onModelAboutToBeReset();

private:
    void clearSavedPaths();

    void saveCurrentPath();
    void saveSelectedPaths();

    QModelIndex restoreCurrentPath();
    QModelIndexList restoreSelectedPaths();

    QStringList treePathForIndex(const QModelIndex &index) const;
    QModelIndex indexForTreePath(const QStringList &path) const;

    QModelIndex findInParent(const QModelIndex &parent, const QString &displayName) const;

    QStringList m_lastCurrentPath;
    QList<QStringList> m_selectedTreePaths;
    const QTreeView *const m_view;
};

} /* namespace TreeViews */
} // namespace Internal
} // namespace Asn1Acn

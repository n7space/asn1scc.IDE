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

#include <QTreeView>

#include "unsynchronizedindexupdater.h"

#include "model.h"

using namespace Asn1Acn::Internal::TreeViews;

UnsynchronizedIndexUpdater::UnsynchronizedIndexUpdater(const QTreeView *view, const Model *model, QObject *parent)
    : IndexUpdater(model, parent)
    , m_view(view)
{
    connect(model, &Model::modelAboutToBeReset,
            this, &UnsynchronizedIndexUpdater::onModelAboutToBeReset);
}

void UnsynchronizedIndexUpdater::onModelAboutToBeReset()
{
    const auto currentIndex = m_view->currentIndex();
    m_lastFocusedItem = currentIndex.isValid() ? currentIndex.data(Qt::DisplayRole).toString() : QString();
}

void UnsynchronizedIndexUpdater::updateCurrentIndex()
{
    const auto matches = m_model->match(m_model->index(0, 0, QModelIndex()),
                                        Qt::DisplayRole,
                                        m_lastFocusedItem,
                                        -1,
                                        Qt::MatchRecursive);

    emit indexSelectionUpdated(matches, matches.value(0));
}

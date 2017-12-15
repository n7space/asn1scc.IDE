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

#include "relationslabelscontroller.h"

#include <QStringList>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

RelationsLabelsController::RelationsLabelsController(MetadataModel *model, QLabel *requires, QLabel *conflicts, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_requires(requires)
    , m_conflicts(conflicts)
{
}

namespace {
QStringList removeSelfContained(const QStringList &internalItems, const QStringList &allItems)
{
    auto ret = internalItems;
    for (const auto &item : allItems)
        ret.removeOne(item);

    return ret;
}
} // namespace Anonymous

void RelationsLabelsController::onFocusedItemChanged(const QModelIndex &current, const QModelIndex &previous) const
{
    Q_UNUSED(previous);

    if (!current.isValid())
        return;

    QStringList provided;
    QStringList required;
    QStringList conflicted;

    fillRelations(current, provided, required, conflicted);

    required.removeDuplicates();
    conflicted.removeDuplicates();

    required = removeSelfContained(required, provided);
    conflicted = removeSelfContained(conflicted, provided);

    m_requires->setText(required.join("\n"));
    m_conflicts->setText(conflicted.join("\n"));
}

void RelationsLabelsController::fillRelations(const QModelIndex &index, QStringList &provides, QStringList &requires, QStringList &conflicts) const
{
    const auto node = m_model->dataNode(index);

    provides << node->name();
    requires << node->requirements();
    conflicts << node->conflicts();

    for (auto i = 0; i < m_model->rowCount(index); ++i)
        for (auto j = 0; j < m_model->columnCount(index); ++j)
            fillRelations(index.child(i, j), provides, requires, conflicts);
}

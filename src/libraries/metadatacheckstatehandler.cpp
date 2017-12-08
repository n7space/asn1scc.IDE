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

#include "metadatacheckstatehandler.h"

#include "metadatamodel.h"

using namespace Asn1Acn::Internal::Libraries;

MetadataCheckStateHandler::MetadataCheckStateHandler(const MetadataModel *model)
    : m_model(model)
{
}

bool MetadataCheckStateHandler::changeCheckStates(const QModelIndex &index, const Qt::CheckState state)
{
    if (state == Qt::Unchecked)
        return uncheck(index);

    if (state == Qt::Checked)
        return check(index, state);

    return true;
}

MetadataCheckStateHandler::States &MetadataCheckStateHandler::changedIndexes()
{
    return m_changedIndexes;
}

MetadataCheckStateHandler::Conflict &MetadataCheckStateHandler::conflict()
{
    return m_conflict;
}

bool MetadataCheckStateHandler::uncheck(const QModelIndex &index)
{
    const auto state = Qt::Unchecked;

    m_changedIndexes.insert(index, state);

    updateChildren(index, state);
    updateParent(index, state);

    return true;
}

bool MetadataCheckStateHandler::check(const QModelIndex &index, const Qt::CheckState state)
{
    m_changedIndexes.insert(index, state);

    return handleRelatives(index, state) && updateChildren(index, state) && updateParent(index, state);
}

bool MetadataCheckStateHandler::updateChildren(const QModelIndex &index, const Qt::CheckState state)
{
    for (auto i = 0; i < m_model->rowCount(index); ++i) {
        for (auto j = 0; j < m_model->columnCount(index); ++j) {
            auto child = index.child(i, j);

            m_changedIndexes.insert(child, state);

            if (state != Qt::Unchecked && !handleRelatives(child, state))
                return false;

            if (!updateChildren(child, state))
                return false;
        }
    }

    return true;
}

bool MetadataCheckStateHandler::updateParent(const QModelIndex &index, const Qt::CheckState childState)
{
    auto state = parentState(index, childState);

    for (auto parent = index.parent(); parent.isValid(); parent = parent.parent()) {
        m_changedIndexes.insert(parent, state);

        if (childState != Qt::Unchecked && !handleRelatives(parent, state))
            return false;

        state = parentState(parent, state);
    }

    return true;
}

bool MetadataCheckStateHandler::handleRelatives(const QModelIndex &index, const Qt::CheckState state)
{
    return noConflicts(index) && checkRequired(index, state);
}

bool MetadataCheckStateHandler::noConflicts(const QModelIndex &index)
{
    const auto currentNode = m_model->dataNode(index);

    for (const auto &item : currentNode->conflicts()) {
        const auto testedNode = m_model->dataNode(findIndexByName(m_model->rootIndex(), item));

        if (testedNode->checked() == Qt::Checked) {
            m_conflict = qMakePair(currentNode->name(), testedNode->name());
            return false;
        }

        for (auto it = m_changedIndexes.begin(); it != m_changedIndexes.end(); ++it)
            if (m_model->dataNode(it.key())->name() == item) {
                m_conflict = qMakePair(currentNode->name(), m_model->dataNode(it.key())->name());
                return false;
            }
    }

    return true;
}

bool MetadataCheckStateHandler::checkRequired(const QModelIndex &index, const Qt::CheckState state)
{
    for (const auto &item : m_model->dataNode(index)->requirements()) {
        auto index = findIndexByName(m_model->rootIndex(), item);
        if (!check(index, state))
            return false;
    }

    return true;
}

QModelIndex MetadataCheckStateHandler::findIndexByName(const QModelIndex &parent, const QString &name) const
{
    if (!parent.isValid())
        return {};

    for (int i = 0; i < m_model->rowCount(parent); ++i) {
        for (int j = 0; j < m_model->columnCount(parent); ++j) {
            auto child = parent.child(i, j);
            if (m_model->dataNode(child)->name() == name)
                return child;

            auto foundInChild = findIndexByName(child, name);
            if (foundInChild.isValid())
                return foundInChild;
        }
    }

    return {};
}

Qt::CheckState MetadataCheckStateHandler::parentState(const QModelIndex &index, const Qt::CheckState state) const
{
    const auto parent = index.parent();

    for (auto i = 0; i < m_model->rowCount(parent); ++i) {
        for (auto j = 0; j < m_model->columnCount(parent); ++j) {
            const auto child = parent.child(i, j);

            if (m_model->data(child, Qt::CheckStateRole) != state
                && (!m_changedIndexes.contains(child) || m_changedIndexes.value(child) != state))
                return Qt::PartiallyChecked;
        }
    }

    return state;
}

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

#include "relationslabelscontroller.h"

#include <QHash>
#include <QStringList>

#include <utils/qtcassert.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

RelationsLabelsController::RelationsLabelsController(MetadataModel *model,
                                                     QTreeWidget *requiresTree,
                                                     QTreeWidget *conflictsTree,
                                                     QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_requiresTree(requiresTree)
    , m_conflictsTree(conflictsTree)
{
}

namespace {
void appendElementItems(const QList<Metadata::Reference> &references, QHash<QString, QTreeWidgetItem *> &parents)
{
    QHash<QString, QTreeWidgetItem *> elements;
    for (const auto &ref : references) {
        const auto name = ref.element();

        QTC_ASSERT(parents.contains(ref.submodule()), continue);

        QTreeWidgetItem *parent = nullptr;
        for (const auto &parentCandidate : parents.values(ref.submodule())) {
            if (parentCandidate->parent()->data(0, Qt::DisplayRole) == ref.module()) {
                parent = parentCandidate;
                break;
            }
        }
        QTC_ASSERT(parent != nullptr, continue);

        if (elements.contains(name) && elements.value(name)->parent() == parent)
            continue;

        elements.insert(name, new QTreeWidgetItem(parent, QStringList(name)));
    }
}

void appendSubmoduleItems(const QList<Metadata::Reference> &references, QHash<QString, QTreeWidgetItem *> &parents)
{
    QHash<QString, QTreeWidgetItem *> submoduleItems;
    for (const auto &ref : references) {
        const auto name = ref.submodule();

        QTC_ASSERT(parents.contains(ref.module()), continue);
        const auto parent = parents.value(ref.module());

        if (submoduleItems.contains(name) && submoduleItems.value(name)->parent() == parent)
            continue;

        submoduleItems.insert(name, new QTreeWidgetItem(parent, QStringList(name)));
    }

    appendElementItems(references, submoduleItems);
}

void appendModuleItems(const QList<Metadata::Reference> &references, QTreeWidget *tree)
{
    QHash<QString, QTreeWidgetItem *> moduleItems;
    for (const auto &ref : references) {
        const auto name = ref.module();
        if (moduleItems.contains(name))
            continue;

        moduleItems.insert(name, new QTreeWidgetItem(tree, QStringList(name)));
    }

    appendSubmoduleItems(references, moduleItems);
}
} // namespace Anonymous

void RelationsLabelsController::onFocusedItemChanged(const QModelIndex &current, const QModelIndex &previous) const
{
    Q_UNUSED(previous);

    if (!current.isValid())
        return;

    m_requiresTree->clear();
    m_conflictsTree->clear();

    QList<Metadata::Reference> requirements;
    QList<Metadata::Reference> conflicts;

    fillRelations(current, requirements, conflicts);

    appendModuleItems(requirements, m_requiresTree);
    m_requiresTree->expandAll();

    appendModuleItems(conflicts, m_conflictsTree);
    m_conflictsTree->expandAll();
}

static void appendReferences(const QList<Metadata::Reference> &source, QList<Metadata::Reference> &target)
{
    for (const auto &item : source)
        if (!target.contains(item))
            target.append(item);
}

void RelationsLabelsController::fillRelations(const QModelIndex &index,
                                              QList<Metadata::Reference> &requirements,
                                              QList<Metadata::Reference> &conflicts) const
{
    const auto node = m_model->dataNode(index);

    appendReferences(node->requirements(), requirements);
    appendReferences(node->conflicts(), conflicts);

    for (auto i = 0; i < m_model->rowCount(index); ++i)
        for (auto j = 0; j < m_model->columnCount(index); ++j)
            fillRelations(index.child(i, j), requirements, conflicts);
}


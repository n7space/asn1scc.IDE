/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
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

#include "typeslocator.h"

#include <QRegularExpression>

#include <coreplugin/editormanager/editormanager.h>

#include <data/definitions.h>
#include <data/file.h>
#include <data/project.h>
#include <data/root.h>

#include <tree-views/decorationrolevisitor.h>

#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;

TypesLocator::TypesLocator()
{
    setShortcutString("asn");
    setDisplayName("ASN.1 type definitions");
    setId("ASN.1 type definitions");

    setIncludedByDefault(false);
}

void TypesLocator::accept(Entry selection,
                          QString *newText,
                          int *selectionStart,
                          int *selectionLength) const
{
    Q_UNUSED(newText);
    Q_UNUSED(selectionStart);
    Q_UNUSED(selectionLength);
    const auto location = qvariant_cast<Data::SourceLocation>(selection.internalData);
    Core::EditorManager::openEditorAt(location.path(), location.line(), location.column());
}

void TypesLocator::refresh(QFutureInterface<void> &future)
{
    Q_UNUSED(future);
}

namespace {

using HighlightInfo = Core::LocatorFilterEntry::HighlightInfo;

class EntriesCollector
{
public:
    using Entry = TypesLocator::Entry;

    EntriesCollector(Core::ILocatorFilter *parent)
        : m_parent(parent)
    {}

    void append(const Data::Node *node,
                const QRegularExpressionMatch &current,
                const QRegularExpressionMatch &parent)
    {
        if (!current.hasMatch() && !parent.hasMatch())
            return;
        auto entry = buildEntry(node);
        entry.highlightInfo = selectHighlight(current, parent);
        selectEntries(current).append(entry);
    }

    QList<Entry> entries() const { return m_betterEntries + m_goodEntries; }

private:
    Entry buildEntry(const Data::Node *node)
    {
        const auto icon = node->valueFor<TreeViews::DecorationRoleVisitor>();
        Entry entry(m_parent, node->name(), QVariant::fromValue(node->location()), icon);
        entry.extraInfo = node->parent()->name();
        return entry;
    }

    QList<Entry> &selectEntries(const QRegularExpressionMatch &current)
    {
        return current.hasMatch() ? m_betterEntries : m_goodEntries;
    }

    HighlightInfo selectHighlight(const QRegularExpressionMatch &current,
                                  const QRegularExpressionMatch &parent)
    {
        return current.hasMatch() ? m_parent->highlightInfo(current)
                                  : m_parent->highlightInfo(parent, HighlightInfo::ExtraInfo);
    }

    Core::ILocatorFilter *m_parent;

    QList<Entry> m_goodEntries;
    QList<Entry> m_betterEntries;
};

} // namespace

QList<TypesLocator::Entry> TypesLocator::matchesFor(QFutureInterface<Entry> &future,
                                                    const QString &entry)
{
    const auto regExp = createRegExp(entry);
    EntriesCollector collector(this);

    for (const auto &project : ParsedDataStorage::instance()->root()->projects())
        for (const auto &file : project->files())
            for (const auto &defs : file->definitionsList()) {
                if (future.isCanceled())
                    return collector.entries();
                const auto defsMatch = regExp.match(defs->name());
                defs->forAllNodes([&](const Data::Node *node) {
                    if (future.isCanceled())
                        return;
                    collector.append(node, regExp.match(node->name()), defsMatch);
                });
            }
    return collector.entries();
}

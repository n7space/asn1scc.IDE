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

#include "typeslocator.h"

#include <QStringMatcher>
#include <QRegExp>

#include <coreplugin/editormanager/editormanager.h>

#include <data/root.h>
#include <data/project.h>
#include <data/file.h>
#include <data/definitions.h>
#include <data/typeassignment.h>
#include <data/variableassignment.h>

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

void TypesLocator::accept(Entry selection, QString *newText, int *selectionStart, int *selectionLength) const
{
    Q_UNUSED(newText); Q_UNUSED(selectionStart); Q_UNUSED(selectionLength);
    const auto location = qvariant_cast<Data::SourceLocation>(selection.internalData);
    Core::EditorManager::openEditorAt(location.path(), location.line(), location.column());
}

void TypesLocator::refresh(QFutureInterface<void> &future)
{
    Q_UNUSED(future);
}

namespace {

using HighlightInfo = Core::LocatorFilterEntry::HighlightInfo;

class StringMatcher
{
public:
    StringMatcher(const QString &entry)
        : m_matcher(entry, Core::ILocatorFilter::caseSensitivity(entry))
        , m_length(entry.length())
    {}

    HighlightInfo match(const QString &text) const
    {
        const auto index = m_matcher.indexIn(text);
        return { index, m_length };
    }

private:
    QStringMatcher m_matcher;
    const int m_length;
};

class WildcardMatcher
{
public:
    WildcardMatcher(const QString &entry)
        : m_matcher(entry, Core::ILocatorFilter::caseSensitivity(entry), QRegExp::Wildcard)
    {}

    HighlightInfo match(const QString &text) const
    {
        const auto index = m_matcher.indexIn(text);
        return { index, m_matcher.matchedLength() };
    }

private:
    QRegExp m_matcher;
};

class Matcher
{
public:
    Matcher(const QString &entry)
        : m_useWildcard(Core::ILocatorFilter::containsWildcard(entry))
        , m_stringMatcher(entry)
        , m_wildcardMatcher(entry)
    {}

    HighlightInfo match(const QString &text) const
    {
        return m_useWildcard ? m_wildcardMatcher.match(text) : m_stringMatcher.match(text);
    }

private:
    bool m_useWildcard;
    StringMatcher m_stringMatcher;
    WildcardMatcher m_wildcardMatcher;
};


class EntriesCollector
{
public:
    using Entry = TypesLocator::Entry;

    EntriesCollector(Core::ILocatorFilter *parent)
        : m_parent(parent)
    {}

    void append(const Data::Node *node, const HighlightInfo &current, const HighlightInfo &parent)
    {
        if (current.startIndex < 0 && parent.startIndex < 0)
            return;
        const auto icon = node->valueFor<TreeViews::DecorationRoleVisitor>();
        Entry entry(m_parent, node->name(), qVariantFromValue(node->location()), icon);
        entry.extraInfo = node->parent()->name();
        if (current.startIndex >= 0) {
            entry.highlightInfo = current;
            if (current.startIndex == 0)
                m_betterEntries.append(entry);
            else
                m_goodEntries.append(entry);
        } else {
            entry.highlightInfo = parent;
            entry.highlightInfo.dataType = HighlightInfo::ExtraInfo;
            m_goodEntries.append(entry);
        }
    }

    QList<Entry> entries() const { return m_betterEntries + m_goodEntries; }

private:
    Core::ILocatorFilter *m_parent;

    QList<Entry> m_goodEntries;
    QList<Entry> m_betterEntries;
};


} // namespace

QList<TypesLocator::Entry> TypesLocator::matchesFor(QFutureInterface<Entry> &future,
                                                    const QString &entry)
{
    const Matcher matcher(entry);
    EntriesCollector collector(this);

    for (const auto &project : ParsedDataStorage::instance()->root()->projects())
        for (const auto &file : project->files())
            for (const auto &defs : file->definitionsList())
            {
                if (future.isCanceled())
                    return {};
                const auto defsMatch = matcher.match(defs->name());
                defs->forAllNodes([&](const Data::Node *node) {
                    if (future.isCanceled())
                        return;
                    collector.append(node, matcher.match(node->name()), defsMatch);
                });
            }
    return collector.entries();
}


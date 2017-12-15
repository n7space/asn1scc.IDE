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

#pragma once

#include <QString>

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Metadata {

class LibraryNode
{
protected:
    LibraryNode(const QString &name, const QString &description)
        : m_name(name)
        , m_description(description)
        , m_parent(nullptr)
        , m_state(Qt::Unchecked)
    {}

public:
    virtual ~LibraryNode() = default;

    const QString &name() const { return m_name; }
    const QString &description() const { return m_description; }

    virtual int childrenCount() const = 0;
    virtual LibraryNode *child(int num) const = 0;
    virtual int childIndex(const LibraryNode *child) const = 0;

    virtual const QStringList conflicts() const { return {}; }
    virtual const QStringList requirements() const { return {}; }

    LibraryNode *parent() const { return m_parent; }
    void setParent(LibraryNode *parent) { m_parent = parent; }

    Qt::CheckState checkState() const { return m_state; }
    void setCheckState(Qt::CheckState state) { m_state = state; }

protected:
    template <typename Collection>
    int findChildIndex(const Collection &col, const LibraryNode *child) const
    {
        int idx = 0;
        for (const auto &item : col) {
            if (item->name() == child->name())
                return idx;
            ++idx;
        }
        return -1;
    }

private:
    const QString m_name;
    const QString m_description;

    LibraryNode *m_parent;
    Qt::CheckState m_state;
};

} // namespace Metadata
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

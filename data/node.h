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

#include <memory>

#include "sourcelocation.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class Node;

using NodePtr = std::shared_ptr<Node>;
using NodeConstPtr = std::shared_ptr<const Node>;

class Node : public std::enable_shared_from_this<Node>
{
protected:
    Node(const SourceLocation& location)
        : m_location(location)
    {}

public:
    virtual ~Node();

    const SourceLocation& location() const { return m_location; }

    NodePtr parent() const { return m_parent.lock(); }
    void setParent(const NodePtr &parent) { m_parent = parent; }

    virtual int childrenCount() const = 0;
    virtual int childIndex(const NodeConstPtr &child) const = 0;
    int indexInParent() const { return parent()->childIndex(shared_from_this()); }

private:
    SourceLocation m_location;
    std::weak_ptr<Node> m_parent;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

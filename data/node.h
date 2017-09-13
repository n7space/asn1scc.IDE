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

#include "sourcelocation.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class Visitor;

class Node
{
protected:
    Node(const SourceLocation& location)
        : m_location(location)
        , m_parent(nullptr)
    {}

public:
    virtual ~Node();

    virtual void accept(Visitor &visitor) const = 0;

    template <typename VisitorType, typename... Args>
    typename VisitorType::ValueType valueFor(Args... args) const
    {
        VisitorType visitor(args...);
        this->accept(visitor);
        return visitor.value();
    }

    const SourceLocation& location() const { return m_location; }

    Node *parent() const { return m_parent; }
    void setParent(Node *parent) { m_parent = parent; }

private:
    SourceLocation m_location;
    Node *m_parent;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

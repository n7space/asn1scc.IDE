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
#include "indexfindingvisitor.h"

#include <utils/qtcassert.h>

#include <data/definitions.h>
#include <data/file.h>

using namespace Asn1Acn::Internal::Data;
using namespace Asn1Acn::Internal::Model;

IndexFindingVisitor::IndexFindingVisitor(const Node *child)
    : m_child(child)
{
}

IndexFindingVisitor::~IndexFindingVisitor()
{
}

template <typename Collection>
int IndexFindingVisitor::findIndexIn(const Collection &items) const
{
    using ValueType = typename Collection::value_type;
    const auto it = std::find_if(std::begin(items), std::end(items),
                                 [this](const ValueType &item){ return item.get() == m_child; });
    QTC_ASSERT(it != items.end(), return -1);
    return std::distance(std::begin(items), it);
}

QVariant IndexFindingVisitor::visit(const Definitions &defs) const
{
    return findIndexIn(defs.types());
}

QVariant IndexFindingVisitor::visit(const File &file) const
{
    return findIndexIn(file.definitionsList());
}

QVariant IndexFindingVisitor::visit(const TypeAssignment &type) const
{
    Q_UNUSED(type);
    return -1;
}

QVariant IndexFindingVisitor::visit(const TypeReference &ref) const
{
    Q_UNUSED(ref);
    return -1;
}

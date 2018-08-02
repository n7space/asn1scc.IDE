/****************************************************************************
**
** Copyright (C) 2017-2018 N7 Space sp. z o. o.
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
#include "childreturningvisitor.h"

#include <tree-views/combo-visitors/childrencountingvisitor.h>

#include <data/types/labeltype.h>
#include <data/types/type.h>

using namespace Asn1Acn::Internal::Data;
using namespace Asn1Acn::Internal::TreeViews::ComboVisitors;

ChildReturningVisitor::ChildReturningVisitor(int index)
    : OutlineVisitors::ChildReturningVisitor(index)
{}

Node *ChildReturningVisitor::valueFor(const File &file) const
{
    // items are static to satisfy unit testing routines
    static TypeAssignment selectSymbol("<Select Symbol>",
                                       {},
                                       std::make_unique<Types::LabelType>("<Select Symbol>"));
    static TypeAssignment noSymbols("<No Symbols>",
                                    {},
                                    std::make_unique<Types::LabelType>("<No Symbols>"));

    if (index() == 0) {
        if (file.valueFor<ChildrenCountingVisitor>() == 1)
            return &noSymbols;

        return &selectSymbol;
    }

    return file.valueFor<OutlineVisitors::ChildReturningVisitor>(index() - 1);
}

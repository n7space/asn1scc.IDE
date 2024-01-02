/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
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
#pragma once

#include <data/node.h>
#include <data/visitorwithvalue.h>

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {
namespace OutlineVisitors {

class ChildReturningVisitor : public Data::VisitorWithValue<Data::Node *>
{
public:
    ChildReturningVisitor(int index);
    ~ChildReturningVisitor() override;

    int index() const { return m_index; }

private:
    Data::Node *valueFor(const Data::Definitions &defs) const override;
    Data::Node *valueFor(const Data::File &file) const override;
    Data::Node *valueFor(const Data::TypeAssignment &type) const override;
    Data::Node *valueFor(const Data::ValueAssignment &value) const override;
    Data::Node *valueFor(const Data::Project &project) const override;
    Data::Node *valueFor(const Data::Root &root) const override;

    int m_index;
};

} // namespace OutlineVisitors
} // namespace TreeViews
} // namespace Internal
} // namespace Asn1Acn

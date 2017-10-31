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

#include <data/visitorwithvalue.h>
#include <data/node.h>

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {
namespace OutlineVisitors {

class IndexFindingVisitor : public Data::VisitorWithValue<int>
{
public:
    IndexFindingVisitor(const Data::Node *child);
    ~IndexFindingVisitor() override;

    const Data::Node *child() const { return m_child; }

private:
    int valueFor(const Data::Definitions &defs) const override;
    int valueFor(const Data::File &file) const override;
    int valueFor(const Data::TypeAssignment &type) const override;
    int valueFor(const Data::TypeReference &ref) const override;
    int valueFor(const Data::Project &project) const override;
    int valueFor(const Data::Root &root) const override;

    const Data::Node *m_child;

    template <typename Collection>
    int findIndexIn(const Collection &items) const;
};

} // namespace OutlineVisitors
} // namespace TreeViews
} // namespace Internal
} // namespace Asn1Acn

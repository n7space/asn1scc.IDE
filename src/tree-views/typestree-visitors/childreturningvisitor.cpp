/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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

#include <utils/qtcassert.h>

#include <data/file.h>
#include <data/project.h>

#include <tree-views/outline-visitors/childrencountingvisitor.h>

using namespace Asn1Acn::Internal::Data;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::TreeViews::TypesTreeVisitors;

ChildReturningVisitor::ChildReturningVisitor(int index)
    : OutlineVisitors::ChildReturningVisitor(index)
{}

ChildReturningVisitor::~ChildReturningVisitor() {}

Node *ChildReturningVisitor::valueFor(const File &file) const
{
    Q_UNUSED(file);
    QTC_ASSERT(false && "This visitor should not be called for Data::File", return nullptr);
}

Node *ChildReturningVisitor::valueFor(const Project &project) const
{
    int idx = index();
    for (const auto &file : project.files()) {
        const int childCount = file->valueFor<OutlineVisitors::ChildrenCountingVisitor>();
        if (idx < childCount)
            return file->valueFor<OutlineVisitors::ChildReturningVisitor>(idx);
        idx -= childCount;
    }
    QTC_ASSERT(false && "Child not found", return nullptr);
}

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
#include "childrencountingvisitor.h"

#include <algorithm>

#include <utils/qtcassert.h>

#include <data/project.h>

using namespace Asn1Acn::Internal::Data;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::TreeViews::TypesTreeVisitors;

ChildrenCountingVisitor::~ChildrenCountingVisitor() {}

int ChildrenCountingVisitor::valueFor(const File &file) const
{
    Q_UNUSED(file);
    QTC_ASSERT(false && "This visitor should not be called for Data::File", return 0);
}

int ChildrenCountingVisitor::valueFor(const Project &project) const
{
    return std::accumulate(project.files().begin(),
                           project.files().end(),
                           0,
                           [](int n, const std::unique_ptr<File> &file) {
                               return n
                                      + file->valueFor<OutlineVisitors::ChildrenCountingVisitor>();
                           });
}

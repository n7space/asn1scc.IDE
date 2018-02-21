/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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
#include "parentreturningvisitor.h"

#include <utils/qtcassert.h>

#include <data/definitions.h>
#include <data/project.h>
#include <data/root.h>
#include <data/valueassignment.h>

using namespace Asn1Acn::Internal::Data;
using namespace Asn1Acn::Internal::TreeViews::TypesTreeVisitors;

ParentReturningVisitor::ParentReturningVisitor() {}

ParentReturningVisitor::~ParentReturningVisitor() {}

Node *ParentReturningVisitor::valueFor(const Definitions &defs) const
{
    const auto file = defs.parent();
    return file ? file->parent() : nullptr;
}

Node *ParentReturningVisitor::valueFor(const File &file) const
{
    Q_UNUSED(file);
    QTC_ASSERT(false && "This visitor should not be called for Data::File", return 0);
}

Node *ParentReturningVisitor::valueFor(const TypeAssignment &type) const
{
    return type.parent();
}

Node *ParentReturningVisitor::valueFor(const ValueAssignment &value) const
{
    return value.parent();
}

Node *ParentReturningVisitor::valueFor(const Project &project) const
{
    return project.parent();
}

Node *ParentReturningVisitor::valueFor(const Root &root) const
{
    return root.parent();
}

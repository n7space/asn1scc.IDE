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
#include "decorationrolevisitor.h"

#include <coreplugin/fileiconprovider.h>

#include <data/definitions.h>
#include <data/file.h>
#include <data/typeassignment.h>
#include <data/variableassignment.h>
#include <data/typereference.h>
#include <data/project.h>

#include <utils/icon.h>

using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::Data;
using namespace Asn1Acn::Internal;

DecorationRoleVisitor::~DecorationRoleVisitor()
{
}

QIcon DecorationRoleVisitor::valueFor(const Definitions &defs) const
{
    Q_UNUSED(defs);
    const static QIcon icon(Utils::Icon({{QLatin1String(":/utils/images/namespace.png"),
                                          Utils::Theme::IconsCodeModelKeywordColor}
                                 }, Utils::Icon::Tint).icon());
    return icon;
}

QIcon DecorationRoleVisitor::valueFor(const File &file) const
{
    Q_UNUSED(file);
    return Core::FileIconProvider::icon(QFileIconProvider::File);
}

QIcon DecorationRoleVisitor::valueFor(const TypeAssignment &type) const
{
    return type.type()->icon(); // TODO mark "type icon"
}

QIcon DecorationRoleVisitor::valueFor(const VariableAssignment &variable) const
{
    return variable.type()->icon(); // TODO mark "variable icon"
}

QIcon DecorationRoleVisitor::valueFor(const Project &project) const
{
    Q_UNUSED(project);
    return Core::FileIconProvider::icon(QFileIconProvider::Folder);
}

QIcon DecorationRoleVisitor::valueFor(const Root &root) const
{
    Q_UNUSED(root);
    return {};
}

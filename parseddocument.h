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

#include <QMultiHash>
#include <QString>
#include <QList>

#include <data/file.h>
#include <data/definitions.h>
#include <data/typereference.h>
#include <data/source.h>

#include "modeltreenode.h"
#include "completion/usertypesproposalsprovider.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDocument
{
public:
    static void bindModelTreeNode(const std::shared_ptr<Data::File> &file, ModelTreeNode::ModelTreeNodePtr moduleNode);

private:
    static ModelTreeNode::ModelTreeNodePtr createDefinition(const Data::Definitions &definition);
    static void attachTypesToDefiniton(const Data::Definitions::Types &types,
                                       ModelTreeNode::ModelTreeNodePtr definitionNode);
};

} // namespace Internal
} // namespace Asn1Acn

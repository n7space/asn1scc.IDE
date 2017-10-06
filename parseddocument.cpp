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

#include "parseddocument.h"

#include <memory>

#include "data/definitions.h"

using namespace Asn1Acn::Internal;

void ParsedDocument::bindModelTreeNode(const std::shared_ptr<Data::File> &file,
                                       ModelTreeNode::ModelTreeNodePtr moduleNode)
{
    if (file == nullptr)
        return;

    for (const auto &definitions : file->definitionsList())
        moduleNode->addChild(createDefinition(*definitions));
}

ModelTreeNode::ModelTreeNodePtr
ParsedDocument::createDefinition(const Data::Definitions &definition)
{
    auto definitionNode = ModelTreeNode::makePtr(definition.name(), Data::Type::UserDefined, definition.location());
    attachTypesToDefiniton(definition.types(), definitionNode);

    return definitionNode;
}

void ParsedDocument::attachTypesToDefiniton(const Data::Definitions::Types &types,
                                            ModelTreeNode::ModelTreeNodePtr definitionNode)
{
    for (const auto& type : types) {
        auto typeNode = ModelTreeNode::makePtr(type->name(), type->reference().type(), type->location());
        definitionNode->addChild(typeNode);
    }
}

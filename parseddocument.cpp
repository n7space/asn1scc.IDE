/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

ParsedDocument::ParsedDocument() :
    m_filePath(QString()),
    m_revision(-1),
    m_parsedData(nullptr)
{
}

ParsedDocument::ParsedDocument(const QString &filePath,
                               int revision,
                               std::unique_ptr<Data::Modules> parsedData) :
    m_filePath(filePath),
    m_revision(revision),
    m_parsedData(std::move(parsedData))
{
}

int ParsedDocument::getRevision() const
{
    return m_revision;
}

void ParsedDocument::bindModelTreeNode(ModelTreeNode::ModelTreeNodePtr moduleNode) const
{
    Data::Modules::DefinitionsMap::const_iterator defIt = m_parsedData->definitions().begin();
    while (defIt != m_parsedData->definitions().end()) {
        auto definitionNode = createDefinition(defIt->second);
        moduleNode->addChild(definitionNode);
        defIt++;
    }
}

ModelTreeNode::ModelTreeNodePtr ParsedDocument::createDefinition(const std::unique_ptr<Data::Definitions> &definition) const
{
    auto definitionNode =
            ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(definition->name(),
                                                              Data::SourceLocation(m_filePath,
                                                                                   definition->location().line(),
                                                                                   definition->location().column())));
    attachTypesToDefiniton(definition->types(), definitionNode);

    return definitionNode;
}

void ParsedDocument::attachTypesToDefiniton(const Data::Definitions::Types types, ModelTreeNode::ModelTreeNodePtr definitionNode) const
{
    Data::Definitions::Types::const_iterator typeIt = types.begin();
    while (typeIt != types.end()) {
        auto typeNode =
                ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(typeIt->second.name(),
                                                                  Data::SourceLocation(m_filePath,
                                                                                       typeIt->second.location().line(),
                                                                                       typeIt->second.location().column())));
        definitionNode->addChild(typeNode);
        typeIt++;
    }
}

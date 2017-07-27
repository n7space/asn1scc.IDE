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

ParsedDocument::ParsedDocument(std::unique_ptr<Data::Modules> parsedData, const DocumentSourceInfo &source) :
    m_source(source),
    m_parsedData(std::move(parsedData))
{
    populateReferences();
}

const DocumentSourceInfo &ParsedDocument::source() const
{
    return m_source;
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

void ParsedDocument::populateReferences()
{
    Data::Modules::DefinitionsMap::const_iterator defIt;
    for (defIt = m_parsedData->definitions().begin(); defIt != m_parsedData->definitions().end(); defIt++)
        populateReferencesFromModule(defIt->second);
}

void ParsedDocument::populateReferencesFromModule(const std::unique_ptr<Data::Definitions> &moduleDefinition)
{
    const Data::Definitions::Types &types = moduleDefinition->types();

    Data::Definitions::Types::const_iterator typeIt;
    for (typeIt = types.begin(); typeIt != types.end(); typeIt++) {
        auto reference = typeIt->second.reference();
        if (reference.type() == Data::TypeReference::DataType::USERDEFINED)
            m_refernceLookup.insert(typeIt->second.location().line(), reference);
    }
}

ModelTreeNode::ModelTreeNodePtr
ParsedDocument::createDefinition(const std::unique_ptr<Data::Definitions> &definition) const
{
    auto definitionNode =
            ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(definition->name(),
                                                              Data::SourceLocation(source().getPath(),
                                                                                   definition->location().line(),
                                                                                   definition->location().column())));
    attachTypesToDefiniton(definition->types(), definitionNode);

    return definitionNode;
}

void ParsedDocument::attachTypesToDefiniton(const Data::Definitions::Types types,
                                            ModelTreeNode::ModelTreeNodePtr definitionNode) const
{
    Data::Definitions::Types::const_iterator typeIt = types.begin();
    while (typeIt != types.end()) {
        auto typeNode =
                ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode(typeIt->second.name(),
                                                                  Data::SourceLocation(source().getPath(),
                                                                                       typeIt->second.location().line(),
                                                                                       typeIt->second.location().column())));
        definitionNode->addChild(typeNode);
        typeIt++;
    }
}

Data::TypeReference ParsedDocument::getTypeReference(const int line, const int col) const
{
    if (!m_refernceLookup.contains(line))
        return Data::TypeReference();

    QList<Data::TypeReference> typeRefs = m_refernceLookup.values(line);
    foreach (const Data::TypeReference &typeRef, typeRefs) {
        Data::SourceLocation sourceLocation = typeRef.location();

        if (col >= sourceLocation.column() && col <= sourceLocation.column() + typeRef.name().size())
            return typeRef;
    }

    return Data::TypeReference();
}

Data::SourceLocation ParsedDocument::getDefinitionLocation(const QString &typeAssignmentName,
                                                           const QString &moduleName) const
{
    Data::Modules::DefinitionsMap::const_iterator defIt;
    for (defIt = m_parsedData->definitions().begin(); defIt != m_parsedData->definitions().end(); defIt++) {

        if (defIt->first != moduleName)
            continue;

        Data::SourceLocation location = getLocationFromModule(defIt->second, typeAssignmentName);
        if (location.isValid())
            return location;
    }

    return Data::SourceLocation();
}

Data::SourceLocation ParsedDocument::getLocationFromModule(const std::unique_ptr<Data::Definitions> &moduleDefinition,
                                                           const QString &typeAssignmentName) const
{
    const Data::Definitions::Types &types = moduleDefinition->types();

    Data::Definitions::Types::const_iterator typeIt;
    for (typeIt = types.begin(); typeIt != types.end(); typeIt++) {
        if (typeIt->first != typeAssignmentName)
            continue;

        return typeIt->second.location();
    }

    return Data::SourceLocation();
}

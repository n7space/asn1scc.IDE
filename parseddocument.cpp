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

ParsedDocument::ParsedDocument(const Data::ModulePtr &parsedData, const DocumentSource &source) :
    m_source(source),
    m_parsedData(parsedData)
{
    populateReferences();
}

ParsedDocument::ParsedDocument(const DocumentSource &source) :
    m_source(source),
    m_parsedData(nullptr)
{
}

const DocumentSource &ParsedDocument::source() const
{
    return m_source;
}

void ParsedDocument::bindModelTreeNode(ModelTreeNode::ModelTreeNodePtr moduleNode) const
{
    if (m_parsedData == nullptr)
        return;

    for (const auto &definitions : m_parsedData->definitionsList())
        moduleNode->addChild(createDefinition(definitions));
}

void ParsedDocument::populateReferences()
{
    if (m_parsedData == nullptr)
        return;

    for (const auto &definitions : m_parsedData->definitionsList())
        populateReferencesFromModule(definitions);
}

void ParsedDocument::populateReferencesFromModule(const Data::DefinitionsPtr &moduleDefinition)
{
    for (const auto &type : moduleDefinition->types()) {
        auto reference = type->reference();
        if (reference.isUserDefined())
            m_referenceLookup.insert(type->location().line(), reference);
    }
}

ModelTreeNode::ModelTreeNodePtr
ParsedDocument::createDefinition(const Data::DefinitionsPtr &definition) const
{
    auto definitionNode = ModelTreeNode::makePtr(definition->name(), Data::Type::UserDefined, buildLocation(definition->location()));
    attachTypesToDefiniton(definition->types(), definitionNode);

    return definitionNode;
}

Data::SourceLocation ParsedDocument::buildLocation(const Data::SourceLocation& location) const
{
    return { source().filePath(), location.line(), location.column() };
}

void ParsedDocument::attachTypesToDefiniton(const Data::Definitions::Types types,
                                            ModelTreeNode::ModelTreeNodePtr definitionNode) const
{
    for (const auto& type : types) {
        auto typeNode = ModelTreeNode::makePtr(type->name(), type->reference().type(), buildLocation(type->location()));
        definitionNode->addChild(typeNode);
    }
}

Data::TypeReference ParsedDocument::getTypeReference(const int line, const int col) const
{
    if (!m_referenceLookup.contains(line))
        return Data::TypeReference();

    QList<Data::TypeReference> typeRefs = m_referenceLookup.values(line);
    foreach (const Data::TypeReference &typeRef, typeRefs) {
        Data::SourceLocation sourceLocation = typeRef.location();

        if (col >= sourceLocation.column() && col <= sourceLocation.column() + typeRef.name().size())
            return typeRef;
    }

    return Data::TypeReference();
}

Data::SourceLocation ParsedDocument::getDefinitionLocation(const QString &typeAssignmentName,
                                                           const QString &definitionsName) const
{
    if (m_parsedData == nullptr)
        return Data::SourceLocation();

    const auto &definitions = m_parsedData->definitions(definitionsName);
    if (!definitions)
        return {};
    return getLocationFromModule(definitions, typeAssignmentName);
}

Data::SourceLocation ParsedDocument::getLocationFromModule(const Data::DefinitionsPtr &moduleDefinition,
                                                           const QString &typeAssignmentName) const
{
    const auto &type = moduleDefinition->type(typeAssignmentName);
    if (!type)
        return {};
    return type->location();
}

Completion::UserTypesProposalsProvider ParsedDocument::getProposalsProvider() const
{
    return Completion::UserTypesProposalsProvider(m_parsedData);
}

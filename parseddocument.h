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

#include "data/module.h"
#include "data/definitions.h"
#include "data/typereference.h"

#include "modeltreenode.h"
#include "documentsource.h"
#include "completion/usertypesproposalsprovider.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDocument
{
public:
    ParsedDocument(const DocumentSource &source = {});
    ParsedDocument(const Data::ModulePtr &parsedData, const DocumentSource &source);

    const DocumentSource &source() const;

    void bindModelTreeNode(ModelTreeNode::ModelTreeNodePtr moduleNode) const;

    Data::TypeReference getTypeReference(const int line, const int col) const;
    Data::SourceLocation getDefinitionLocation(const QString& typeAssignmentName, const QString& definitionsName) const;

    Completion::UserTypesProposalsProvider getProposalsProvider() const;

private:
    ModelTreeNode::ModelTreeNodePtr createDefinition(const Data::DefinitionsPtr &definition) const;
    void attachTypesToDefiniton(const Data::Definitions::Types types,
                                ModelTreeNode::ModelTreeNodePtr definitionNode) const;

    void populateReferences();
    void populateReferencesFromModule(const Data::DefinitionsPtr &moduleDefinition);

    Data::SourceLocation getLocationFromModule(const Data::DefinitionsPtr &definition,
                                               const QString &typeAssignmentName) const;
    Data::SourceLocation buildLocation(const Data::SourceLocation& location) const;

    DocumentSource m_source;
    Data::ModulePtr m_parsedData;

    QMultiHash<int, Data::TypeReference> m_referenceLookup;
};

} // namespace Internal
} // namespace Asn1Acn

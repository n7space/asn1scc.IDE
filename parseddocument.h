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

#include "data/modules.h"
#include "data/definitions.h"
#include "data/typereference.h"

#include "modeltreenode.h"
#include "documentsourceinfo.h"
#include "completion/usertypesproposalsprovider.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDocument
{
public:
    ParsedDocument(const DocumentSourceInfo &source = {});
    ParsedDocument(std::unique_ptr<Data::Modules> parsedData, const DocumentSourceInfo &source);

    const DocumentSourceInfo &source() const;

    void bindModelTreeNode(ModelTreeNode::ModelTreeNodePtr moduleNode) const;

    Data::TypeReference getTypeReference(const int line, const int col) const;
    Data::SourceLocation getDefinitionLocation(const QString& typeAssignmentName, const QString& moduleName) const;

    Completion::UserTypesProposalsProvider getProposalsProvider() const;

private:
    ModelTreeNode::ModelTreeNodePtr createDefinition(const std::unique_ptr<Data::Definitions> &definition) const;
    void attachTypesToDefiniton(const Data::Definitions::Types types,
                                ModelTreeNode::ModelTreeNodePtr definitionNode) const;

    void populateReferences();
    void populateReferencesFromModule(const std::unique_ptr<Data::Definitions> &moduleDefinition);

    Data::SourceLocation getLocationFromModule(const std::unique_ptr<Data::Definitions> &definition,
                                               const QString &typeAssignmentName) const;
    Data::SourceLocation buildLocation(const Data::SourceLocation& location) const;

    DocumentSourceInfo m_source;
    std::unique_ptr<Data::Modules> m_parsedData;

    QMultiHash<int, Data::TypeReference> m_refernceLookup;
};

} // namespace Internal
} // namespace Asn1Acn

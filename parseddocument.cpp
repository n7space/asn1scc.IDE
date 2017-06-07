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

#include <memory>

#include "parseddocument.h"

using namespace Asn1Acn::Internal;

ParsedDocument::ParsedDocument() :
    m_filePath(QString()),
    m_revision(-1)
{
}

ParsedDocument::ParsedDocument(const QString &filePath, int revision, const QStringList &wordList) :
    m_filePath(filePath),
    m_revision(revision)
{
    m_parsedTreeRoot = std::shared_ptr<ParsedObject>(new ParsedObject(m_filePath));

    foreach (const QString &entry, wordList)
        m_parsedTreeRoot->addChild(std::shared_ptr<ParsedObject>(new ParsedObject(QVariant(entry))));
}

ParsedDocument::ParsedDocument(const QString &filePath,
                               int revision,
                               std::unique_ptr<Data::Modules> parsedData) :
    m_filePath(filePath),
    m_revision(revision)
{
    m_parsedTreeRoot = std::shared_ptr<ParsedObject>(new ParsedObject(m_filePath));

    using DefinitionsMap = Data::Modules::DefinitionsMap;
    using Types = Data::Definitions::Types;

    DefinitionsMap::const_iterator defIt = parsedData->definitions().begin();
    while (defIt != parsedData->definitions().end()) {
        QString definitionName = defIt->first;
        auto definition = std::shared_ptr<ParsedObject>(new ParsedObject(definitionName));

        const Types &types = defIt->second->types();
        Types::const_iterator typeIt = types.begin();
        while (typeIt != types.end()) {
            QString typeName = typeIt->first;
            definition->addChild(std::shared_ptr<ParsedObject>(new ParsedObject(typeName)));
            typeIt++;
        }

        m_parsedTreeRoot->addChild(definition);
        defIt++;
    }
}

int ParsedDocument::getRevision() const
{
    return m_revision;
}

std::shared_ptr<ParsedObject> ParsedDocument::getTreeRoot()
{
    return m_parsedTreeRoot;
}

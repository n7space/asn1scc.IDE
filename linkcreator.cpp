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

#include "linkcreator.h"

#include <texteditor/textdocument.h>
#include <texteditor/convenience.h>

#include <utils/fileutils.h>

#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;

LinkCreator::LinkCreator(const TextEditor::TextDocument &document)
    : m_documentPath(document.filePath().toString()), m_textDocument(document)
{
    ParsedDataStorage *storage = ParsedDataStorage::instance();
    m_parsedDocument = storage->getFileForPath(m_documentPath);
    if (m_parsedDocument == nullptr)
        m_parsedDocument = std::make_shared<ParsedDocument>();
}

LinkCreator::Link LinkCreator::createHighlightLink(const QTextCursor &cursor) const
{
    Data::TypeReference typeRef = getSymbolTypeReference(cursor);

    return getSymbolLink(typeRef, cursor);
}

LinkCreator::Link LinkCreator::createTargetLink(const QTextCursor &cursor) const
{
    Data::TypeReference typeRef = getSymbolTypeReference(cursor);

    Link sourceSymbol = getSymbolLink(typeRef, cursor);

    return getTargetSymbolLink(typeRef, sourceSymbol);
}

Data::TypeReference LinkCreator::getSymbolTypeReference(const QTextCursor &cursor) const
{
    if (!m_textDocument.characterAt(cursor.position()).isLetterOrNumber())
        return Data::TypeReference();

    return m_parsedDocument->getTypeReference(cursor.blockNumber() + 1, cursor.columnNumber());
}

LinkCreator::Link LinkCreator::getSymbolLink(const Data::TypeReference &symbolSource, const QTextCursor &cursor) const
{
    Link symbol(m_documentPath);

    QTextCursor selection = TextEditor::Convenience::selectAt(cursor,
                                                              symbolSource.location().line(),
                                                              symbolSource.location().column(),
                                                              symbolSource.name().length());

    symbol.linkTextStart = selection.selectionStart() + 1;
    symbol.linkTextEnd = selection.selectionEnd() + 1;

    return symbol;
}

LinkCreator::Link LinkCreator::getTargetSymbolLink(const Data::TypeReference &symbolSource, const Link &symbol) const
{
    Link target = symbol;

    Data::SourceLocation targetLocation = m_parsedDocument->getDefinitionLocation(symbolSource.name(),
                                                                                  symbolSource.module());

    if (!targetLocation.isValid()) {
        targetLocation = getTargetLocation(symbolSource.name(), symbolSource.module());
        target.targetFileName = targetLocation.path();
    }

    target.targetLine = targetLocation.line();
    target.targetColumn = targetLocation.column();

    return target;
}

Data::SourceLocation LinkCreator::getTargetLocation(const QString &typeName, const QString &moduleName) const
{
    QStringList projects = ParsedDataStorage::instance()->getProjectsForFile(m_documentPath);

    if (projects.empty())
        return Data::SourceLocation();

    Data::SourceLocation location;
    for (const QString &projectName : projects) {
        location = getTargetLocationFromProject(projectName, typeName, moduleName);
        if (location.isValid())
            return location;
    }

    return Data::SourceLocation();
}

Data::SourceLocation LinkCreator::getTargetLocationFromProject(const QString &projectName,
                                                               const QString &typeName,
                                                               const QString &moduleName) const
{
    QList<std::shared_ptr<ParsedDocument>> documents = ParsedDataStorage::instance()->getFilesFromProject(projectName);
    for (const std::shared_ptr<ParsedDocument> &document : documents) {
        if (document->source().getPath() == m_documentPath)
            continue;

        const auto location = document->getDefinitionLocation(typeName, moduleName);

        // can not simply return location, as it contains only file name and not file path
        if (location.isValid())
            return Data::SourceLocation(document->source().getPath(), location.line(), location.column());
    }

    return Data::SourceLocation();
}

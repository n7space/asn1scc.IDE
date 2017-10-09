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

    return ParsedDataStorage::instance()->getTypeReference(m_documentPath,
                                                           cursor.blockNumber() + 1,
                                                           cursor.columnNumber());
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

    Data::SourceLocation targetLocation = ParsedDataStorage::instance()->getDefinitionLocation(m_documentPath,
                                                                                               symbolSource.name(),
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
    const auto storage = ParsedDataStorage::instance();

    const auto paths = storage->getFilesPathsFromProject(projectName);
    for (const auto &path : paths) {
        if (path == m_documentPath)
            continue;

        const auto location = storage->getDefinitionLocation(path, typeName, moduleName);

        // TODO: does comment below holds true?
        // can not simply return location, as it contains only file name and not file path
        if (location.isValid())
            return Data::SourceLocation(path, location.line(), location.column());
    }

    return Data::SourceLocation();
}

/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include <utils/fileutils.h>
#include <utils/textutils.h>

#include "referencefinder.h"

using namespace Asn1Acn::Internal;

LinkCreator::LinkCreator(const TextEditor::TextDocument &document, const ParsedDataStorage *storage)
    : m_textDocument(document)
    , m_storage(storage)
{}

Utils::Link LinkCreator::createHighlightLink(const QTextCursor &cursor) const
{
    const auto typeRef = ReferenceFinder(m_textDocument, m_storage).findAt(cursor);
    return getSymbolLink(typeRef, cursor);
}

Utils::Link LinkCreator::createTargetLink(const QTextCursor &cursor) const
{
    const auto typeRef = ReferenceFinder(m_textDocument, m_storage).findAt(cursor);

    Utils::Link sourceSymbol = getSymbolLink(typeRef, cursor);

    return getTargetSymbolLink(typeRef, sourceSymbol);
}

Utils::Link LinkCreator::getSymbolLink(const Data::TypeReference &symbolSource,
                                       const QTextCursor &cursor) const
{
    Utils::Link symbol(m_textDocument.filePath());

    QTextCursor selection = Utils::Text::selectAt(cursor,
                                                  symbolSource.location().line(),
                                                  symbolSource.location().column() + 1,
                                                  symbolSource.name().length());

    symbol.linkTextStart = selection.selectionStart();
    symbol.linkTextEnd = selection.selectionEnd();

    return symbol;
}

Utils::Link LinkCreator::getTargetSymbolLink(const Data::TypeReference &symbolSource,
                                             const Utils::Link &symbol) const
{
    Utils::Link target = symbol;

    Data::SourceLocation targetLocation = m_storage->getDefinitionLocation(m_textDocument.filePath(),
                                                                           symbolSource.name(),
                                                                           symbolSource.module());

    if (!targetLocation.isValid()) {
        targetLocation = getTargetLocation(symbolSource.name(), symbolSource.module());
        target.targetFilePath = Utils::FilePath::fromString(targetLocation.path());
    }

    target.targetLine = targetLocation.line();
    target.targetColumn = targetLocation.column();

    return target;
}

Data::SourceLocation LinkCreator::getTargetLocation(const QString &typeName,
                                                    const QString &moduleName) const
{
    const auto projects = m_storage->getProjectsForFile(m_textDocument.filePath());

    for (const QString &projectName : projects) {
        const auto location = getTargetLocationFromProject(projectName, typeName, moduleName);
        if (location.isValid())
            return location;
    }

    return Data::SourceLocation();
}

Data::SourceLocation LinkCreator::getTargetLocationFromProject(const QString &projectName,
                                                               const QString &typeName,
                                                               const QString &moduleName) const
{
    const auto paths = m_storage->getFilesPathsFromProject(projectName);
    for (const auto &path : paths) {
        if (path == m_textDocument.filePath())
            continue;

        const auto location = m_storage->getDefinitionLocation(path, typeName, moduleName);
        if (location.isValid())
            return location;
    }

    return {};
}

/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
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
#include "referencefinder.h"

#include <texteditor/textdocument.h>
#include <utils/fileutils.h>

#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;

ReferenceFinder::ReferenceFinder(const TextEditor::TextDocument &document,
                                 const ParsedDataStorage *storage)
    : m_textDocument(document)
    , m_storage(storage)
{}

Data::TypeReference ReferenceFinder::findAt(const QTextCursor &cursor) const
{
    if (!isProperAsn1IdentifierChar(m_textDocument.characterAt(cursor.position())))
        return Data::TypeReference();

    const auto file = m_storage->getAnyFileForPath(m_textDocument.filePath());
    return findAt(file, cursor.blockNumber() + 1, cursor.columnNumber());
}

Data::TypeReference ReferenceFinder::findAt(const Data::File *file, int line, int col) const
{
    if (file == nullptr)
        return Data::TypeReference();

    const auto range = file->referencesMap().equal_range(line);
    for (auto it = range.first; it != range.second; it++) {
        const auto referedColumn = it->second->location().column();
        const auto size = it->second->name().size();
        if (col >= referedColumn && col <= referedColumn + size)
            return *(it->second);
    }

    return Data::TypeReference();
}

bool ReferenceFinder::isProperAsn1IdentifierChar(QChar ch) const
{
    return ch.isLetterOrNumber() || ch == '-';
}

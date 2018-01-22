/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
** Contact: http://n7space.com
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

#include <QTextCursor>
#include <QChar>

#include <texteditor/texteditor.h>

#include "data/file.h"
#include "data/typereference.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDataStorage;

class ReferenceFinder
{
public:
    ReferenceFinder(const TextEditor::TextDocument &document,
                    const ParsedDataStorage *storage);

    Data::TypeReference findAt(const QTextCursor &cursor) const;

private:
    bool isProperAsn1IdentifierChar(QChar ch) const;
    Data::TypeReference findAt(const Data::File *file, int line, int col) const;
    const TextEditor::TextDocument &m_textDocument;
    const ParsedDataStorage *m_storage;
};

} /* namespace Internal */
} /* namespace Asn1Acn */

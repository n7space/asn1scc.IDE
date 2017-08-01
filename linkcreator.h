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

#pragma once

#include "memory"

#include <QString>
#include <QTextCursor>

#include <texteditor/texteditor.h>

#include "data/typereference.h"
#include "data/sourcelocation.h"

#include "parseddocument.h"

namespace Asn1Acn {
namespace Internal {

class LinkCreator
{
public:

    using Link = TextEditor::TextEditorWidget::Link;

    LinkCreator(const TextEditor::TextDocument &document);

    Link createHighlightLink(const QTextCursor &cursor) const;
    Link createTargetLink(const QTextCursor &cursor) const;

private:
    Data::TypeReference getSymbolTypeReference(const QTextCursor &cursor) const;
    Link getSymbolLink(const Data::TypeReference &symbolSource, const QTextCursor &cursor) const;

    Link getTargetSymbolLink(const Data::TypeReference &symbolSource, const Link &symbol) const;

    Data::SourceLocation getTargetLocation(const QString &typeName, const QString &moduleName) const;
    Data::SourceLocation getTargetLocationFromProject(const QString &projectName,
                                                      const QString &typeName,
                                                      const QString &moduleName) const;

    const QString m_documentPath;
    std::shared_ptr<ParsedDocument> m_parsedDocument;
    const TextEditor::TextDocument &m_textDocument;
};

} /* namespace Internal */
} /* namespace Asn1Acn */

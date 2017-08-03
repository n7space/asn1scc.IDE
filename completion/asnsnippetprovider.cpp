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

#include "asnsnippetprovider.h"

#include "autocompleter.h"
#include "../asn1acnconstants.h"

#include <texteditor/snippets/snippeteditor.h>
#include <texteditor/textdocument.h>

#include <QLatin1String>
#include <QCoreApplication>

using namespace Asn1Acn::Internal::Completion;

QString AsnSnippetProvider::groupId() const
{
    return QLatin1String(Constants::ASN1_SNIPPETS_GROUP_ID);
}

QString AsnSnippetProvider::displayName() const
{
    return QCoreApplication::translate("Asn1Acn::Internal::AsnSnippetProvider", "ASN.1");
}

void AsnSnippetProvider::decorateEditor(TextEditor::SnippetEditorWidget *editor) const
{
    editor->setAutoCompleter(new AutoCompleter);
    editor->textDocument()->setMimeType(Constants::ASN1_MIMETYPE);
    editor->configureGenericHighlighter();
}

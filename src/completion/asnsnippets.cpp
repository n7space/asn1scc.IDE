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
#include "asnsnippets.h"

#include <QLatin1String>
#include <QCoreApplication>

#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>
#include <texteditor/snippets/snippetprovider.h>

#include <asn1acnconstants.h>

#include "autocompleter.h"
#include "tr.h"

using namespace Asn1Acn::Internal::Completion;

static void decorateEditor(TextEditor::TextEditorWidget *editor)
{
    editor->setAutoCompleter(new AutoCompleter);
    editor->textDocument()->setMimeType(Asn1Acn::Constants::ASN1_MIMETYPE);
    editor->configureGenericHighlighter();
}

void AsnSnippets::registerGroup()
{
    TextEditor::SnippetProvider::registerGroup(QLatin1String(Constants::ASN1_SNIPPETS_GROUP_ID),
                                               Tr::tr("ASN.1"),
                                               &decorateEditor);
}

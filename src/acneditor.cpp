/****************************************************************************
**
** Copyright (C) 2017-2018 N7 Space sp. z o. o.
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

#include "acneditor.h"

#include <QCoreApplication>

#include <texteditor/basehoverhandler.h>
#include <texteditor/texteditoractionhandler.h>

#include "acndocument.h"
#include "asn1acnconstants.h"
#include "completion/acncompletionassist.h"
#include "completion/autocompleter.h"
#include "indenter.h"
#include "tr.h"

using namespace Asn1Acn::Internal;

using namespace Core;
using namespace TextEditor;

AcnEditor::AcnEditor()
{
    addContext(Constants::LANG_ACN);
    addContext(Constants::BASE_CONTEXT);
}

AcnEditorFactory::AcnEditorFactory()
{
    setId(Constants::ACNEDITOR_ID);
    setDisplayName(Tr::tr("ACN Editor"));
    addMimeType(Constants::ACN_MIMETYPE);

    setDocumentCreator([]() { return new AcnDocument; });
    setEditorWidgetCreator([]() { return new AcnEditorWidget; });
    setEditorCreator([]() { return new AcnEditor; });

    setCompletionAssistProvider(new Completion::AcnCompletionAssistProvider);
    setIndenterCreator([]() { return new Indenter; });
    setAutoCompleterCreator([]() { return new Completion::AutoCompleter; });

    addHoverHandler(new TextEditor::BaseHoverHandler); // TODO maybe better tips?

    setCodeFoldingSupported(true);
    setMarksVisible(true);
    setParenthesesMatchingEnabled(true);
    setUseGenericHighlighter(true);

    setEditorActionHandlers(TextEditorActionHandler::Format
                            | TextEditorActionHandler::UnCommentSelection
                            | TextEditorActionHandler::UnCollapseAll
                            | TextEditorActionHandler::FollowSymbolUnderCursor);
}

void AcnEditorWidget::findUsages()
{
    // TODO acn
}

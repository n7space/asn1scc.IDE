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

#include "asneditor.h"

#include <QApplication>

#include <texteditor/texteditoractionhandler.h>
#include <texteditor/basehoverhandler.h>

#include "completion/autocompleter.h"
#include "completion/asncompletionassist.h"
#include "filesourcereader.h"
#include "asn1acnconstants.h"
#include "linkcreator.h"
#include "referencefinder.h"
#include "parseddatastorage.h"
#include "usagesfinder.h"
#include "asndocument.h"
#include "indenter.h"
#include "tr.h"

using namespace Asn1Acn::Internal;

using namespace Core;
using namespace TextEditor;

AsnEditor::AsnEditor()
{
    addContext(Constants::LANG_ASN1);
    addContext(Constants::BASE_CONTEXT);
}

AsnEditorFactory::AsnEditorFactory()
{
    setId(Constants::ASNEDITOR_ID);
    setDisplayName(Tr::tr("ASN.1 Editor"));
    addMimeType(Constants::ASN1_MIMETYPE);

    setDocumentCreator([]() { return new AsnDocument; });
    setEditorWidgetCreator([]() { return new AsnEditorWidget; });
    setEditorCreator([]() { return new AsnEditor; });

    setCompletionAssistProvider(new Completion::AsnCompletionAssistProvider);
    setAutoCompleterCreator([]() { return new Completion::AutoCompleter; });
    setIndenterCreator([]() { return new Indenter; });

    addHoverHandler(new TextEditor::BaseHoverHandler); // TODO maybe better hover tips?

    setCodeFoldingSupported(true);
    setMarksVisible(true);
    setParenthesesMatchingEnabled(true);
    setUseGenericHighlighter(true);

    setEditorActionHandlers(TextEditorActionHandler::Format
                          | TextEditorActionHandler::UnCommentSelection
                          | TextEditorActionHandler::UnCollapseAll
                          | TextEditorActionHandler::FollowSymbolUnderCursor);
}

AsnEditorWidget::AsnEditorWidget()
    : m_usagesFinder(new UsagesFinder(ParsedDataStorage::instance(),
                                      std::make_unique<FileSourceReader>(),
                                      this))
{
}

AsnEditorWidget::Link AsnEditorWidget::findLinkAt(const QTextCursor &cursor,
                                                  bool resolveTarget,
                                                  bool inNextSplit)
{
    Q_UNUSED(inNextSplit);

    LinkCreator linkCreator(*textDocument(), ParsedDataStorage::instance());
    if (resolveTarget)
        return linkCreator.createTargetLink(cursor);
    else
        return linkCreator.createHighlightLink(cursor);
}

void AsnEditorWidget::findUsages()
{
    ReferenceFinder refFinder(*textDocument(), ParsedDataStorage::instance());
    const auto ref = refFinder.findAt(textCursor());
    if (ref.location().isValid())
        m_usagesFinder->findUsages(ref.module(), ref.name());
}

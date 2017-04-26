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

#include "asneditor.h"

#include <QApplication>

#include <texteditor/texteditoractionhandler.h>

#include "asn1acnconstants.h"
#include "asndocument.h"

using namespace Asn1Acn::Internal;

AsnEditor::AsnEditor()
{
    addContext(Constants::LANG_ASN1);
}

AsnEditorFactory::AsnEditorFactory()
{
    setId(Constants::ASNEDITOR_ID);
    setDisplayName(QCoreApplication::translate("OpenWith::Editors", Constants::ASNEDITOR_DISPLAY_NAME));
    addMimeType(Constants::ASN1_MIMETYPE);

    setDocumentCreator([]() { return new AsnDocument; });
    setEditorWidgetCreator([]() { return new AsnEditorWidget; });
    setEditorCreator([]() { return new AsnEditor; });
        // TODO setAutoCompleterCreator([]() { return new CppAutoCompleter; });
    setCodeFoldingSupported(true);
    setMarksVisible(true);
    setParenthesesMatchingEnabled(true);

    using namespace TextEditor;
    setEditorActionHandlers(TextEditorActionHandler::Format
                          | TextEditorActionHandler::UnCommentSelection
                          | TextEditorActionHandler::UnCollapseAll
                          | TextEditorActionHandler::FollowSymbolUnderCursor);

  // TODO      addHoverHandler(new CppHoverHandler);
}

AsnEditorWidget::AsnEditorWidget()
{
 // TODO ? setLanguageSettingsId(Constants::SettingsId);

    // TODO will probably be extracted to some asnacn base class
    m_commentDefinition.multiLineStart.clear();
    m_commentDefinition.multiLineEnd.clear();
    m_commentDefinition.singleLine = QLatin1Literal("--");
}

void AsnEditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this, m_commentDefinition);
}

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
#include "autocompleter.h"

#include <texteditor/textdocumentlayout.h>

#include <QTextCursor>

using namespace Asn1Acn::Internal::Completion;

AutoCompleter::AutoCompleter()
{
}

bool AutoCompleter::isInComment(const QTextCursor &cursor) const
{
    // TODO: This doesn't handle '--' inside quotes
    QTextCursor moved = cursor;
    moved.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    return moved.selectedText().contains(QLatin1Literal("--"));
}

bool AutoCompleter::isInString(const QTextCursor &cursor) const
{
    // TODO: multiline strings in ASN?
    // TODO: escaping string?
    QTextCursor moved = cursor;
    moved.movePosition(QTextCursor::StartOfLine);
    const int positionInLine = cursor.position() - moved.position();
    moved.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    const QString line = moved.selectedText();

    bool isEscaped = false;
    bool inString = false;
    for (int i = 0; i < positionInLine; ++i) {
        const QChar c = line.at(i);
        if (c == QLatin1Char('\\') && !isEscaped)
            isEscaped = true;
        else if (c == QLatin1Char('"') && !isEscaped)
            inString = !inString;
        else
            isEscaped = false;
    }
    return inString;
}

QString AutoCompleter::insertMatchingBrace(const QTextCursor &cursor,
                                           const QString &text,
                                           QChar lookAhead,
                                           bool skipChars,
                                           int *skippedChars) const
{
    Q_UNUSED(cursor)
    if (text.isEmpty())
        return QString();
    const QChar current = text.at(0);
    switch (current.unicode()) {
    case '(':
        return QStringLiteral(")");

    case ')':
        if (current == lookAhead && skipChars)
            ++*skippedChars;
        break;

    default:
        break;
    }

    return QString();
}

QString AutoCompleter::insertMatchingQuote(const QTextCursor &cursor,
                                           const QString &text,
                                           QChar lookAhead,
                                           bool skipChars,
                                           int *skippedChars) const
{
    Q_UNUSED(cursor)
    const QChar quote(QLatin1Char('"'));
    if (text.isEmpty() || text != quote)
        return QString();
    if (lookAhead == quote && skipChars) {
        ++*skippedChars;
        return QString();
    }
    return quote;
}

QString AutoCompleter::insertParagraphSeparator(const QTextCursor &cursor) const
{
    Q_UNUSED(cursor);
    return QLatin1String("}");
}

bool AutoCompleter::contextAllowsAutoBrackets(const QTextCursor &cursor,
                                              const QString &textToInsert) const
{
    Q_UNUSED(textToInsert);
    return !isInComment(cursor) && !isInString(cursor);
}

bool AutoCompleter::contextAllowsAutoQuotes(const QTextCursor &cursor, const QString &textToInsert) const
{
    return contextAllowsAutoBrackets(cursor, textToInsert);
}

int AutoCompleter::paragraphSeparatorAboutToBeInserted(QTextCursor &cursor,
                                                       const TextEditor::TabSettings &tabSettings)
{
    auto block = cursor.document()->lastBlock();
    TextEditor::TextDocumentLayout::setBraceDepth(block, 1); // TODO - workaround to reuse code from base class
    return TextEditor::AutoCompleter::paragraphSeparatorAboutToBeInserted(cursor, tabSettings);
}

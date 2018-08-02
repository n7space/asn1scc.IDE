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
#include "autocompleter.h"

#include <texteditor/textdocumentlayout.h>

#include <QTextCursor>

using namespace Asn1Acn::Internal::Completion;

bool AutoCompleter::isInComment(const QTextCursor &cursor) const
{
    QTextCursor moved = cursor;

    int idx = findWordIndexInCurrentLine(moved, QLatin1String("--"));
    if (idx == -1)
        return false;

    moveCursorInCurrentLine(moved, idx);

    return !isInString(moved);
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
    const QChar quote(QLatin1Char('"'));

    if (text != quote)
        return QString();

    if (lookAhead == quote && skipChars) {
        ++*skippedChars;
        return QString();
    }

    if (isInString(cursor))
        return QString();

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

bool AutoCompleter::contextAllowsAutoQuotes(const QTextCursor &cursor,
                                            const QString &textToInsert) const
{
    Q_UNUSED(textToInsert);
    return !isInComment(cursor);
}

int AutoCompleter::paragraphSeparatorAboutToBeInserted(QTextCursor &cursor)
{
    if (tryInsertEndKeyword(cursor))
        return 1;

    if (!bracesBalanced(cursor.document()->firstBlock())) {
        setBraceDepthInBlocks(cursor.block(), 1);
        return TextEditor::AutoCompleter::paragraphSeparatorAboutToBeInserted(cursor);
    }

    return 0;
}

bool AutoCompleter::tryInsertEndKeyword(QTextCursor &cursor) const
{
    if (!shouldInsertEndKeyword(cursor))
        return false;

    insertEndKeyword(cursor);

    return true;
}

bool AutoCompleter::shouldInsertEndKeyword(QTextCursor &cursor) const
{
    return containsBeginKeyword(cursor) && contextAllowsEndKeyword(cursor)
           && beginKeywordMismatched(cursor);
}

bool AutoCompleter::containsBeginKeyword(QTextCursor &cursor) const
{
    QTextCursor moved = cursor;

    moved.select(QTextCursor::LineUnderCursor);

    QString line = moved.selectedText();
    QStringList words = line.trimmed().split(" ");

    return words.last() == QLatin1String("BEGIN");
}

bool AutoCompleter::contextAllowsEndKeyword(const QTextCursor &cursor) const
{
    QTextCursor moved = cursor;

    const int idx = findWordIndexInCurrentLine(moved, QLatin1String("BEGIN"));
    if (idx == -1)
        return false;

    moveCursorInCurrentLine(moved, idx);

    return !isInComment(moved) && !isInString(moved);
}

bool AutoCompleter::beginKeywordMismatched(QTextCursor &cursor) const
{
    QTextCursor moved = cursor;

    while (moved.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor)) {
        QString text = moved.selectedText();
        moved.clearSelection();

        if (isInComment(moved) || isInString(moved))
            continue;

        if (text == QLatin1String("END"))
            return false;

        if (text == QLatin1String("BEGIN"))
            return true;
    }

    return true;
}

void AutoCompleter::insertEndKeyword(QTextCursor &cursor) const
{
    const int pos = cursor.position();

    cursor.insertBlock();
    cursor.insertText(QLatin1String("END"));
    cursor.setPosition(pos);
}

int AutoCompleter::findWordIndexInCurrentLine(const QTextCursor &cursor,
                                              const QLatin1String &word) const
{
    QTextCursor moved = cursor;
    moved.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);

    return moved.selectedText().indexOf(word);
}

void AutoCompleter::moveCursorInCurrentLine(QTextCursor &cursor, const int position) const
{
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, position);
}

bool AutoCompleter::bracesBalanced(const QTextBlock &firstBlock)
{
    int totalBalance = 0;

    for (auto block = firstBlock; block.isValid(); block = block.next())
        totalBalance += bracesBalanceInBlock(block);

    return totalBalance <= 0;
}

int AutoCompleter::bracesBalanceInBlock(const QTextBlock &block)
{
    int balance = 0;

    for (const auto &p : TextEditor::TextDocumentLayout::parentheses(block)) {
        QTextCursor cursor(block);
        moveCursorInCurrentLine(cursor, p.pos);

        if (isInComment(cursor) || isInString(cursor))
            continue;

        p.type == TextEditor::Parenthesis::Opened ? ++balance : --balance;
    }

    return balance;
}

void AutoCompleter::setBraceDepthInBlocks(const QTextBlock &firstBlock, int depth)
{
    for (auto block = firstBlock; block.isValid(); block = block.next())
        TextEditor::TextDocumentLayout::setBraceDepth(block, depth);
}

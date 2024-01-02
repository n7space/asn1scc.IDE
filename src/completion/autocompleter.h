/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
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

#pragma once

#include <texteditor/autocompleter.h>

namespace Asn1Acn {
namespace Internal {
namespace Completion {

class AutoCompleter : public TextEditor::AutoCompleter
{
public:
    bool isInComment(const QTextCursor &cursor) const override;
    bool isInString(const QTextCursor &cursor) const override;
    QString insertMatchingBrace(const QTextCursor &cursor,
                                const QString &text,
                                QChar lookAhead,
                                bool skipChars,
                                int *skippedChars) const override;
    QString insertMatchingQuote(const QTextCursor &cursor,
                                const QString &text,
                                QChar lookAhead,
                                bool skipChars,
                                int *skippedChars) const override;
    QString insertParagraphSeparator(const QTextCursor &cursor) const override;
    int paragraphSeparatorAboutToBeInserted(QTextCursor &cursor) override;
    bool contextAllowsAutoBrackets(const QTextCursor &cursor,
                                   const QString &textToInsert) const override;
    bool contextAllowsAutoQuotes(const QTextCursor &cursor,
                                 const QString &textToInsert) const override;

private:
    bool tryInsertEndKeyword(QTextCursor &cursor) const;
    bool shouldInsertEndKeyword(QTextCursor &cursor) const;
    bool containsBeginKeyword(QTextCursor &cursor) const;
    bool contextAllowsEndKeyword(const QTextCursor &cursor) const;
    bool beginKeywordMismatched(QTextCursor &cursor) const;
    void insertEndKeyword(QTextCursor &cursor) const;

    int findWordIndexInCurrentLine(const QTextCursor &cursor, const QLatin1String &word) const;
    void moveCursorInCurrentLine(QTextCursor &cursor, const int position) const;

    bool bracesBalanced(const QTextBlock &firstBlock);
    int bracesBalanceInBlock(const QTextBlock &block);
    void setBraceDepthInBlocks(const QTextBlock &firstBlock, int depth);
};

} /* namespace Completion */
} /* namespace Internal */
} /* namespace Asn1Acn */

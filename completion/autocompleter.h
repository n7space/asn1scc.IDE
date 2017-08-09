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

#pragma once

#include <texteditor/autocompleter.h>

namespace Asn1Acn {
namespace Internal {
namespace Completion {

class AutoCompleter : public TextEditor::AutoCompleter
{
public:
    AutoCompleter();

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
    int paragraphSeparatorAboutToBeInserted(QTextCursor &cursor,
                                            const TextEditor::TabSettings &tabSettings) override;
    bool contextAllowsAutoBrackets(const QTextCursor &cursor, const QString &textToInsert) const override;
    bool contextAllowsAutoQuotes(const QTextCursor &cursor, const QString &textToInsert) const override;
};

} /* nameapsce Completion */
} /* namespace Internal */
} /* namespace Asn1Acn */

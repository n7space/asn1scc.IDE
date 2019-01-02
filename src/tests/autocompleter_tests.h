/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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

#pragma once

#include <QObject>

#include <completion/autocompleter.h>

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class AutoCompleterTests : public QObject
{
    Q_OBJECT
public:
    explicit AutoCompleterTests(QObject *parent = 0);
    ~AutoCompleterTests();

private slots:
    void test_cursorInComment();
    void test_cursorNotInComment();

    void test_cursorInString();
    void test_cursorNotInString();
    void test_cursorInEscapedString();

    void test_cursorInCommentInString();
    void test_cursorNotInCommentInString();

    void test_insertMatchingBraceEmptyString();
    void test_insertMatchingBraceForLeftBrace();
    void test_insertMatchingBraceForRigthBrace();
    void test_insertMatchingBraceForText();

    void test_insertMatchingQuoteEmptyString();
    void test_insertMatchingQuoteForText();
    void test_insertMatchingQuoteForSkippedQuote();
    void test_insertMatchingQuoteForQuote();
    void test_insertQuoteInsideMatchedQuotes();

    void test_insertParagraphSepareator();

    void test_insertEndForBegin();
    void test_insertEndForBeginAlreadyPaired();
    void test_insertEndForCommentedBegin();
    void test_insertEndForBeginInString();
    void test_insertEndForBeginNotInTheEndOfLine();
    void test_insertEndForBeginWhenEndIsCommented();

    void test_insertCurlyBraceMatch();
    void test_insertCurlyBraceMismatch();
    void test_insertCurlyBraceMatchInComment();

private:
    Completion::AutoCompleter *m_completer;
};

} // namespace Tests
} // namespace Internal
} // namespace Asn1Acn

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

#include "autocompleter_tests.h"

#include <QtTest>

#include <QTextCursor>
#include <QTextDocument>

#include <texteditor/tabsettings.h>
#include <texteditor/textdocumentlayout.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

AutoCompleterTests::AutoCompleterTests(QObject *parent)
    : QObject(parent)
{
    m_completer = new Completion::AutoCompleter;
}

AutoCompleterTests::~AutoCompleterTests()
{
    delete m_completer;
}

void AutoCompleterTests::test_cursorInString()
{
    QTextDocument *document = new QTextDocument("\"Inside string\"");
    QTextCursor cursor(document);
    cursor.setPosition(5);

    QVERIFY(m_completer->isInString(cursor));

    delete document;
}

void AutoCompleterTests::test_cursorNotInString()
{
    QTextDocument *document = new QTextDocument("Not inside string");
    QTextCursor cursor(document);
    cursor.setPosition(5);

    QVERIFY(!m_completer->isInString(cursor));

    delete document;
}

void AutoCompleterTests::test_cursorInEscapedString()
{
    QTextDocument *document = new QTextDocument("Escaped \\ string");
    QTextCursor cursor(document);
    cursor.setPosition(12);

    QVERIFY(!m_completer->isInString(cursor));

    delete document;
}

void AutoCompleterTests::test_cursorInCommentInString()
{
    QTextDocument *document = new QTextDocument("\"Text not commented -- text commented\"");
    QTextCursor cursor(document);
    cursor.setPosition(25);

    QVERIFY(m_completer->isInString(cursor));
    QVERIFY(!m_completer->isInComment(cursor));

    delete document;
}

void AutoCompleterTests::test_cursorNotInCommentInString()
{
    QTextDocument *document = new QTextDocument("\"Text not commented -- text commented\"");
    QTextCursor cursor(document);
    cursor.setPosition(10);

    QVERIFY(m_completer->isInString(cursor));
    QVERIFY(!m_completer->isInComment(cursor));

    delete document;
}

void AutoCompleterTests::test_cursorInComment()
{
    QTextDocument *document = new QTextDocument("Not in comment -- In comment");
    QTextCursor cursor(document);
    cursor.setPosition(20);

    QVERIFY(m_completer->isInComment(cursor));

    delete document;
}

void AutoCompleterTests::test_cursorNotInComment()
{
    QTextDocument *document = new QTextDocument("Not in comment -- In comment");
    QTextCursor cursor(document);
    cursor.setPosition(5);

    QVERIFY(!m_completer->isInComment(cursor));

    delete document;
}

void AutoCompleterTests::test_insertMatchingBraceEmptyString()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingBrace(QTextCursor(),
                                                   QString(),
                                                   QChar(),
                                                   true,
                                                   &skippedChars);

    QCOMPARE(ret, QString());
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingBraceForLeftBrace()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingBrace(QTextCursor(),
                                                   QString("("),
                                                   QChar(),
                                                   true,
                                                   &skippedChars);

    QCOMPARE(ret, QStringLiteral(")"));
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingBraceForRigthBrace()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingBrace(QTextCursor(),
                                                   QString(")"),
                                                   QChar(')'),
                                                   true,
                                                   &skippedChars);

    QCOMPARE(ret, QString());
    QCOMPARE(skippedChars, 6);
}

void AutoCompleterTests::test_insertMatchingBraceForText()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingBrace(QTextCursor(),
                                                   QString("Test string"),
                                                   QChar(')'),
                                                   true,
                                                   &skippedChars);

    QCOMPARE(ret, QString());
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingQuoteEmptyString()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingQuote(QTextCursor(),
                                                   QString(),
                                                   QChar(),
                                                   true,
                                                   &skippedChars);

    QCOMPARE(ret, QString());
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingQuoteForText()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingQuote(QTextCursor(),
                                                   QString("Test string"),
                                                   QChar('"'),
                                                   true,
                                                   &skippedChars);

    QCOMPARE(ret, QString());
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingQuoteForSkippedQuote()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingQuote(QTextCursor(),
                                                   QString("\""),
                                                   QChar('"'),
                                                   true,
                                                   &skippedChars);

    QCOMPARE(ret, QString());
    QCOMPARE(skippedChars, 6);
}

void AutoCompleterTests::test_insertMatchingQuoteForQuote()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingQuote(QTextCursor(),
                                                   QString("\""),
                                                   QChar(),
                                                   false,
                                                   &skippedChars);

    QCOMPARE(ret, QString("\""));
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertQuoteInsideMatchedQuotes()
{
    int skippedChars = 0;
    QTextDocument *document = new QTextDocument("\"\"");
    QTextCursor cursor(document);
    cursor.setPosition(1);

    QString ret = m_completer->insertMatchingQuote(cursor,
                                                   QString("\""),
                                                   QChar(),
                                                   false,
                                                   &skippedChars);

    QCOMPARE(ret, QString());

    delete document;
}

void AutoCompleterTests::test_insertParagraphSepareator()
{
    QString ret = m_completer->insertParagraphSeparator(QTextCursor());
    QCOMPARE(ret, QString("}"));
}

void AutoCompleterTests::test_insertEndForBegin()
{
    QString text("Autocomplete DEFINITIONS ::= BEGIN");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QString expected = text + QString("\nEND");

    QCOMPARE(ret, 1);
    QCOMPARE(document->toPlainText(), expected);

    delete document;
}

void AutoCompleterTests::test_insertEndForBeginAlreadyPaired()
{
    QString text("Autocomplete DEFINITIONS ::= BEGIN\n"
                 "MyDef ::= INTEGER\n"
                 "END\n");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QString expected = text;

    QCOMPARE(ret, 0);
    QCOMPARE(document->toPlainText(), expected);

    delete document;
}

void AutoCompleterTests::test_insertEndForCommentedBegin()
{
    QString text("-- Autocomplete DEFINITIONS ::= BEGIN");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QString expected = text;

    QCOMPARE(ret, 0);
    QCOMPARE(document->toPlainText(), expected);

    delete document;
}

void AutoCompleterTests::test_insertEndForBeginInString()
{
    QString text("\"Autocomplete DEFINITIONS ::= BEGIN\"");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QString expected = text;

    QCOMPARE(ret, 0);
    QCOMPARE(document->toPlainText(), expected);

    delete document;
}

void AutoCompleterTests::test_insertEndForBeginNotInTheEndOfLine()
{
    QString text("\"AutoBEGIN DEFINITIONS ::= \"");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QString expected = text;

    QCOMPARE(ret, 0);
    QCOMPARE(document->toPlainText(), expected);

    delete document;
}

void AutoCompleterTests::test_insertEndForBeginWhenEndIsCommented()
{
    QString text("Autocomplete DEFINITIONS ::= BEGIN\n"
                 "MyDef ::= INTEGER\n"
                 "-- END\n");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QString expected("Autocomplete DEFINITIONS ::= BEGIN\n"
                     "END\n"
                     "MyDef ::= INTEGER\n"
                     "-- END\n");

    QCOMPARE(ret, 1);
    QCOMPARE(document->toPlainText(), expected);

    delete document;
}

void AutoCompleterTests::test_insertCurlyBraceMatch()
{
    QString text("{"
                 "\n");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    TextEditor::TextDocumentLayout::setParentheses(
        document->firstBlock(),
        TextEditor::Parentheses() << TextEditor::Parenthesis(TextEditor::Parenthesis::Type::Opened,
                                                             QLatin1Char('{'),
                                                             0));

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QString expected("{\n"
                     "}"
                     "\n");

    QCOMPARE(ret, 1);
    QCOMPARE(document->toPlainText(), expected);

    delete document;
}

void AutoCompleterTests::test_insertCurlyBraceMismatch()
{
    QString text("{\n"
                 "}\n"
                 "\n");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    TextEditor::TextDocumentLayout::setParentheses(
        document->firstBlock(),
        TextEditor::Parentheses() << TextEditor::Parenthesis(TextEditor::Parenthesis::Type::Opened,
                                                             QLatin1Char('{'),
                                                             0));
    TextEditor::TextDocumentLayout::setParentheses(
        document->firstBlock().next(),
        TextEditor::Parentheses() << TextEditor::Parenthesis(TextEditor::Parenthesis::Type::Closed,
                                                             QLatin1Char('}'),
                                                             0));

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QString expected = text;

    QCOMPARE(ret, 0);
    QCOMPARE(document->toPlainText(), expected);
}

void AutoCompleterTests::test_insertCurlyBraceMatchInComment()
{
    QString text("{\n"
                 "-- }\n"
                 "\n");

    QTextDocument *document = new QTextDocument(text);
    QTextCursor cursor(document);
    cursor.movePosition(QTextCursor::EndOfLine);

    TextEditor::TextDocumentLayout::setParentheses(
        document->firstBlock(),
        TextEditor::Parentheses() << TextEditor::Parenthesis(TextEditor::Parenthesis::Type::Opened,
                                                             QLatin1Char('{'),
                                                             0));
    TextEditor::TextDocumentLayout::setParentheses(
        document->firstBlock().next(),
        TextEditor::Parentheses() << TextEditor::Parenthesis(TextEditor::Parenthesis::Type::Closed,
                                                             QLatin1Char('}'),
                                                             4));

    QString expected("{\n"
                     "}\n"
                     "-- }\n"
                     "\n");

    int ret = m_completer->paragraphSeparatorAboutToBeInserted(cursor);

    QCOMPARE(ret, 1);
    QCOMPARE(document->toPlainText(), expected);
}

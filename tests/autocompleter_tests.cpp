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

#include "autocompleter_tests.h"

#include <QtTest>

#include <QTextCursor>
#include <QTextDocument>

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
    QString ret = m_completer->insertMatchingBrace(QTextCursor(), QString(), QChar(), true, &skippedChars);

    QCOMPARE(QString(), ret);
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingBraceForLeftBrace()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingBrace(QTextCursor(), QString("("), QChar(), true, &skippedChars);

    QCOMPARE(QStringLiteral(")"), ret);
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingBraceForRigthBrace()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingBrace(QTextCursor(), QString(")"), QChar(')'), true, &skippedChars);

    QCOMPARE(QString(), ret);
    QCOMPARE(skippedChars, 6);
}

void AutoCompleterTests::test_insertMatchingBraceForText()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingBrace(QTextCursor(), QString("Test string"), QChar(')'), true, &skippedChars);

    QCOMPARE(QString(), ret);
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingQuoteEmptyString()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingQuote(QTextCursor(), QString(), QChar(), true, &skippedChars);

    QCOMPARE(QString(), ret);
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingQuoteForText()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingQuote(QTextCursor(), QString("Test string"), QChar(')'), true, &skippedChars);

    QCOMPARE(QString(), ret);
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertMatchingQuoteForSkippedQuote()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingQuote(QTextCursor(), QString("\""), QChar('"'), true, &skippedChars);

    QCOMPARE(QString(), ret);
    QCOMPARE(skippedChars, 6);
}

void AutoCompleterTests::test_insertMatchingQuoteForQuote()
{
    int skippedChars = 5;
    QString ret = m_completer->insertMatchingQuote(QTextCursor(), QString("\""), QChar(), false, &skippedChars);

    QCOMPARE(QString("\""), ret);
    QCOMPARE(skippedChars, 5);
}

void AutoCompleterTests::test_insertParagraphSepareator()
{
    QString ret = m_completer->insertParagraphSeparator(QTextCursor());
    QCOMPARE(QString("}"), ret);
}

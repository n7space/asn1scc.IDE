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
#include "indenter_tests.h"

#include <QtTest>

#include <QTextBlock>
#include <QTextDocument>

#include <texteditor/tabsettings.h>

using namespace Asn1Acn::Internal::Tests;

static const int TAB_SIZE = 4;
static const int INDENT_SIZE = 4;

IndenterTests::IndenterTests(QObject *parent)
    : QObject(parent)
{}

void IndenterTests::initTestCase()
{
    m_indenter = new Indenter;

    m_document = new QTextDocument;
    m_document->setPlainText(
                /*  0 */  "    TestingAutocompletion DEFINITIONS ::= BEGIN\n"
                /*  1 */  "    EXPORTS ALL;\n"
                /*  2 */  "    IMPORTS\n"
                /*  3 */  "        MyInt FROM MyModelName;\n"
                /*  4 */  "        \n"
                /*  5 */  "    MySequence ::= SEQUENCE {\n"
                /*  6 */  "        a INTEGER\n"
                /*  7 */  "    }\n"
                /*  8 */  "    \n"
                /*  9 */  "        OtherSequence ::= SEQUENCE\n"
                /* 10 */  "        {\n"
                /* 11 */  "            \n"
                /* 12 */  "            \n"
                /* 13 */  "            b INTEGER\n"
                /* 14 */  "            \n"
                /* 15 */  "            \n"
                /* 16 */  "        }\n"
                /* 17 */  "    \n"
                /* 18 */  "    END\n"
                );


    m_tabSettings = new TextEditor::TabSettings(TextEditor::TabSettings::SpacesOnlyTabPolicy,
                                                4,
                                                4,
                                                TextEditor::TabSettings::NoContinuationAlign);
}

void IndenterTests::cleanupTestCase()
{
    delete m_indenter;
    delete m_document;
    delete m_tabSettings;
}

void IndenterTests::test_curlyBracketsAreElectric()
{
    Indenter indenter;

    QVERIFY(indenter.isElectricCharacter(QLatin1Char('{')));
    QVERIFY(indenter.isElectricCharacter(QLatin1Char('}')));
}

void IndenterTests::test_firstLine()
{
    const auto block = m_document->findBlockByLineNumber(0);
    const int indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, 0);
}

void IndenterTests::test_normalNonEmptyLine()
{
    const auto block = m_document->findBlockByLineNumber(1);
    const int indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, INDENT_SIZE);
}

void IndenterTests::test_normalEmptyLine()
{
    const auto block = m_document->findBlockByLineNumber(4);
    const int indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, 2 * INDENT_SIZE);
}

void IndenterTests::test_bracketCloseLine()
{
    auto block = m_document->findBlockByLineNumber(7);
    int indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, INDENT_SIZE);

    block = m_document->findBlockByLineNumber(16);
    indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, 2 * INDENT_SIZE);
}

void IndenterTests::test_afterBracketCloseLine()
{
    auto block = m_document->findBlockByLineNumber(17);
    int indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, INDENT_SIZE);
}

void IndenterTests::test_afterBracketOpenLine()
{
    auto block = m_document->findBlockByLineNumber(6);
    int indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, 2 * INDENT_SIZE);

    block = m_document->findBlockByLineNumber(11);
    indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, 3 * INDENT_SIZE);
}

void IndenterTests::test_bracketOpenInManuallyIndentedNonEmptyLine()
{
    auto block = m_document->findBlockByLineNumber(5);
    int indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, INDENT_SIZE);
}

void IndenterTests::test_afterBracketOpenAndNewLinesLine()
{
    auto block = m_document->findBlockByLineNumber(13);
    int indent = m_indenter->indentFor(block, *m_tabSettings);

    QCOMPARE(indent, 3 * INDENT_SIZE);
}

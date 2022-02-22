/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
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

#include "selectionpositionresolver_test.h"

#include <QTextCursor>

#include <texteditor/textdocument.h>
#include <utils/fileutils.h>

#include <QtTest>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

SelectionPositionResolverTests::SelectionPositionResolverTests(QObject *parent)
    : QObject(parent)
    , m_doc(std::make_unique<TextEditor::TextDocument>())
{}

QTextCursor SelectionPositionResolverTests::getInitializedCursor(const QByteArray &content,
                                                                 const int position)
{
    m_doc->setContents(content);
    m_doc->setFilePath(Utils::FilePath::fromString("myPath"));

    QTextCursor cursor(m_doc->document());
    cursor.setPosition(position);

    return cursor;
}

void SelectionPositionResolverTests::test_emptyDocument()
{
    auto cursor = getInitializedCursor("", 0);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 0);
}

void SelectionPositionResolverTests::test_singleWord()
{
    auto cursor = getInitializedCursor("AAAA BBBB CCCC", 6);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 5);
    QCOMPARE(m_resolver.endPosition(), 9);
}

void SelectionPositionResolverTests::test_singleWordOnlyHyphens()
{
    auto cursor = getInitializedCursor("---- ---- ----", 6);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 5);
    QCOMPARE(m_resolver.endPosition(), 9);
}

void SelectionPositionResolverTests::test_wordWithHyphenInMiddle()
{
    auto cursor = getInitializedCursor("AAAA-BBBB-CCCC", 6);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 14);
}

void SelectionPositionResolverTests::test_wordWithMultipleHyhpens()
{
    auto cursor = getInitializedCursor("AAAA-----BBBB-----CCCC", 1);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 22);
}

void SelectionPositionResolverTests::test_selectHyphensAsPartOfWord()
{
    auto cursor = getInitializedCursor("AAAA-----BBBB-----CCCC", 6);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 22);
}

void SelectionPositionResolverTests::test_wordWithHyphensAtEnd()
{
    auto cursor = getInitializedCursor("AAAA---------", 1);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 13);
}

void SelectionPositionResolverTests::test_selectHyphensAsWordEnding()
{
    auto cursor = getInitializedCursor("AAAA---------", 6);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 13);
}

void SelectionPositionResolverTests::test_wordWithHyphensAtStart()
{
    auto cursor = getInitializedCursor("---------AAAA", 12);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 13);
}

void SelectionPositionResolverTests::test_selectHyphensAsWordStarting()
{
    auto cursor = getInitializedCursor("---------AAAA", 2);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 13);
}

void SelectionPositionResolverTests::test_wordPrecededWithHyphensOnlyWord()
{
    auto cursor = getInitializedCursor("--------- AAAA", 2);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 9);
}

void SelectionPositionResolverTests::test_wordFollowedByHyphensOnlyWord()
{
    auto cursor = getInitializedCursor("AAAA ---------", 2);

    m_resolver.resolve(cursor);
    QCOMPARE(m_resolver.startPosition(), 0);
    QCOMPARE(m_resolver.endPosition(), 4);
}

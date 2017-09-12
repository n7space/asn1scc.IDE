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
#include "errormessageparser_tests.h"

#include <QtTest>

#include "../errormessageparser.h"

using namespace Asn1Acn::Internal::Tests;
using namespace Asn1Acn::Internal;

ErrorMessageParserTests::ErrorMessageParserTests(QObject *parent)
    : QObject(parent)
{}

void ErrorMessageParserTests::test_emptyMessage()
{
    const auto parser = ErrorMessageParser();
    const auto msg = parser.parse(QString());

    QVERIFY(!msg.isValid());
}

void ErrorMessageParserTests::test_wrongFormatMessage()
{
    const auto parser = ErrorMessageParser();
    const auto msg = parser.parse("a:b:c");

    QVERIFY(!msg.isValid());
}

void ErrorMessageParserTests::test_messageWithOnlyLineNumber()
{
    const auto parser = ErrorMessageParser();
    const auto msg = parser.parse("xyz.asn:123: error: Message Text");

    QVERIFY(msg.isValid());
    QCOMPARE(msg.location().path(), QStringLiteral("xyz.asn"));
    QCOMPARE(msg.location().line(), 123);
    QCOMPARE(msg.location().column(), 0);
}

void ErrorMessageParserTests::test_messageWithColumnAndLineNumber()
{
    const auto parser = ErrorMessageParser();
    const auto msg = parser.parse("xyz.asn:123:987: error: Message Text");

    QVERIFY(msg.isValid());
    QCOMPARE(msg.location().path(), QStringLiteral("xyz.asn"));
    QCOMPARE(msg.location().line(), 123);
    QCOMPARE(msg.location().column(), 987);
}

void ErrorMessageParserTests::test_mappingSourceFiles()
{
    const auto parser = ErrorMessageParser(PathMapper({ {"/abc/xyz.asn", ""} }));
    const auto msg = parser.parse("xyz.asn:123:987: error: Message Text");

    QVERIFY(msg.isValid());
    QCOMPARE(msg.location().path(), QStringLiteral("/abc/xyz.asn"));
    QCOMPARE(msg.location().line(), 123);
    QCOMPARE(msg.location().column(), 987);
}

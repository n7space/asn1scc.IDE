/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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
#include "generalmetadataparser_tests.h"

#include <QtTest>

using namespace Asn1Acn::Internal::Libraries::Tests;

GeneralMetadataParserTests::GeneralMetadataParserTests(QObject *parent)
    : QObject(parent)
    , m_parsedData("BadName", "BadPath")
{
}

void GeneralMetadataParserTests::test_empty()
{
    parse("", "/some/path/with/LibName");

    QCOMPARE(m_parsedData.name(), QLatin1Literal("LibName"));
    QCOMPARE(m_parsedData.path(), QLatin1Literal("/some/path/with/LibName"));
    QVERIFY(m_parsedData.license().isEmpty());
    QVERIFY(m_parsedData.version().isEmpty());
    QVERIFY(m_parsedData.description().isEmpty());
}

void GeneralMetadataParserTests::test_partiallyFilled()
{
    parse(R"({"name": "InnerName", "license": "GPL"})", "/some/path/with/LibName");

    QCOMPARE(m_parsedData.name(), QLatin1Literal("InnerName"));
    QCOMPARE(m_parsedData.path(), QLatin1Literal("/some/path/with/LibName"));
    QCOMPARE(m_parsedData.license(), QLatin1Literal("GPL"));
    QVERIFY(m_parsedData.version().isEmpty());
    QVERIFY(m_parsedData.description().isEmpty());
}

void GeneralMetadataParserTests::test_complete()
{
    parse(R"({)"
          R"("name": "InnerName",)"
          R"("version": "SomeVersion",)"
          R"("vendor": "MyVendor",)"
          R"("description": "SomeDescription",)"
          R"("license": "GPL"})",
          "/some/path/with/LibName");

    QCOMPARE(m_parsedData.name(), QLatin1Literal("InnerName"));
    QCOMPARE(m_parsedData.path(), QLatin1Literal("/some/path/with/LibName"));
    QCOMPARE(m_parsedData.license(), QLatin1Literal("GPL"));
    QCOMPARE(m_parsedData.version(), QLatin1Literal("SomeVersion"));
    QCOMPARE(m_parsedData.description(), QLatin1Literal("SomeDescription"));
    QCOMPARE(m_parsedData.vendor(), QLatin1Literal("MyVendor"));
}

void GeneralMetadataParserTests::parse(const QString &jsonData, const QString &path)
{
    GeneralMetadataParser parser(jsonData.toUtf8(), path);
    m_parsedData = parser.parse();
}

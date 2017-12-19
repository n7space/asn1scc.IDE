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
#include "modulemetadataparser_tests.h"

#include <QtTest>

using namespace Asn1Acn::Internal::Libraries::Tests;

ModuleMetadataParserTests::ModuleMetadataParserTests(QObject *parent)
    : QObject(parent),
      m_parsedData(std::make_unique<Metadata::Module>("BadName", "BadDesc"))
{
}

void ModuleMetadataParserTests::test_emptyFile()
{
    parsingFails("");
}

void ModuleMetadataParserTests::test_malformedJson()
{
    parsingFails(R"( { "name": "xxxx", )");
}

void ModuleMetadataParserTests::test_wrongJsonType()
{
    parsingFails("[]");
}

void ModuleMetadataParserTests::test_emptyObject()
{
    parsingFails("{}");
}

void ModuleMetadataParserTests::test_emptyModule()
{
    parse(R"({"name": "SomeName", "description": "SomeDesc"})");

    QCOMPARE(m_parsedData->name(), QLatin1Literal("SomeName"));
    QCOMPARE(m_parsedData->description(), QLatin1Literal("SomeDesc"));
    QCOMPARE(static_cast<int>(m_parsedData->submodules().size()), 0);
}

void ModuleMetadataParserTests::test_emptySubmodule()
{
    parse(R"({
          "name": "SomeName",
          "submodules": [
              {
                "name": "SubmoduleName",
                "description": "SubmoduleDesc"
              }
            ]
          })");

    QCOMPARE(static_cast<int>(m_parsedData->submodules().size()), 1);
    const auto submodule = m_parsedData->submodules().at(0).get();
    QCOMPARE(submodule->name(), QLatin1Literal("SubmoduleName"));
    QCOMPARE(submodule->description(), QLatin1Literal("SubmoduleDesc"));
    QCOMPARE(static_cast<int>(submodule->elements().size()), 0);
}

void ModuleMetadataParserTests::test_emptyElement()
{
    parse(R"({
          "name": "SomeName",
          "submodules": [
              {
                "name": "SubmoduleName",
                "elements": [
                  {
                    "name": "ElementName",
                    "description": "ElementDesc"
                  }
                ]
              }
            ]
          })");

    QCOMPARE(static_cast<int>(m_parsedData->submodules().size()), 1);
    const auto submodule = m_parsedData->submodules().at(0).get();
    QCOMPARE(static_cast<int>(submodule->elements().size()), 1);
    const auto element = submodule->elements().at(0).get();
    QCOMPARE(element->name(), QLatin1Literal("ElementName"));
    QCOMPARE(element->description(), QLatin1Literal("ElementDesc"));
    QCOMPARE(element->asn1Files().size(), 0);
    QCOMPARE(element->conflicts().size(), 0);
    QCOMPARE(element->requirements().size(), 0);
}

void ModuleMetadataParserTests::test_completeElement()
{
    parse(R"({
          "name": "SomeName",
          "submodules": [
              {
                "name": "SubmoduleName",
                "elements": [
                  {
                    "name": "ElementName",
                    "asn1Files": ["f1", "f2"],
                    "conflicts": ["c1", "c2", "c3"],
                    "requires": ["r1"],
                  }
                ]
              }
            ]
          })");

    QCOMPARE(static_cast<int>(m_parsedData->submodules().size()), 1);
    const auto submodule = m_parsedData->submodules().at(0).get();
    QCOMPARE(static_cast<int>(submodule->elements().size()), 1);
    const auto element = submodule->elements().at(0).get();
    QCOMPARE(element->asn1Files(), (QStringList{"f1", "f2"}));
    QCOMPARE(element->conflicts(), (QStringList{"c1", "c2", "c3"}));
    QCOMPARE(element->requirements(), (QStringList{"r1"}));
}

void ModuleMetadataParserTests::parsingFails(const QString &jsonData)
{
    ModuleMetadataParser parser(jsonData.toUtf8());
    try {
        parser.parse();
        QFAIL("Parsing should fail");
    }
    catch (const ModuleMetadataParser::Error &err) {
        QVERIFY(QLatin1String(err.what()).size() > 0);
    }
}

void ModuleMetadataParserTests::parse(const QString &jsonData)
{
    ModuleMetadataParser parser(jsonData.toUtf8());
    m_parsedData = parser.parse();
}

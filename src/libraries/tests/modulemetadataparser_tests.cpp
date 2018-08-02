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
#include "modulemetadataparser_tests.h"

#include <QtTest>

using namespace Asn1Acn::Internal::Libraries::Tests;

ModuleMetadataParserTests::ModuleMetadataParserTests(QObject *parent)
    : QObject(parent)
    , m_parsedData(std::make_unique<Metadata::Module>("BadName", "BadDesc"))
{}

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
    QCOMPARE(element->additionalFiles().size(), 0);
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
                    "asn1Files": ["f1", "f2", "f3"],
                    "additionalFiles": ["a1", "a2", "a3"],
                    "conflicts": ["c1", "c2"],
                    "requires": ["r1"]
                  }
                ]
              }
            ]
          })");

    QCOMPARE(static_cast<int>(m_parsedData->submodules().size()), 1);
    const auto submodule = m_parsedData->submodules().at(0).get();
    QCOMPARE(static_cast<int>(submodule->elements().size()), 1);
    const auto element = submodule->elements().at(0).get();

    QCOMPARE(element->asn1Files(), (QStringList{"f1", "f2", "f3"}));
    QCOMPARE(element->additionalFiles(), (QStringList{"a1", "a2", "a3"}));

    QCOMPARE(element->conflicts().size(), 2);
    QCOMPARE(element->conflicts().at(0).element(), QStringLiteral("c1"));
    QCOMPARE(element->conflicts().at(0).submodule(), QStringLiteral("SubmoduleName"));
    QCOMPARE(element->conflicts().at(0).module(), QStringLiteral("SomeName"));
    QCOMPARE(element->conflicts().at(1).element(), QStringLiteral("c2"));
    QCOMPARE(element->conflicts().at(1).submodule(), QStringLiteral("SubmoduleName"));
    QCOMPARE(element->conflicts().at(1).module(), QStringLiteral("SomeName"));

    QCOMPARE(element->requirements().size(), 1);
    QCOMPARE(element->requirements().at(0).element(), QStringLiteral("r1"));
    QCOMPARE(element->requirements().at(0).submodule(), QStringLiteral("SubmoduleName"));
    QCOMPARE(element->requirements().at(0).module(), QStringLiteral("SomeName"));
}

void ModuleMetadataParserTests::test_complexReferences()
{
    parse(R"({
          "name": "SomeName",
          "submodules": [
              {
                "name": "SubmoduleName",
                "elements": [
                  {
                    "name": "ElementName",
                    "asn1Files": ["f1"],
                    "conflicts": [{
                                    "module": "OtherModule",
                                    "submodule": "OtherSubmodule",
                                    "element": "ConflictingElement"
                                 }],
                    "requires": [{
                                    "submodule": "LocalSubmodule",
                                    "element": "RequiredElement"
                                }]
                  }
                ]
              }
            ]
          })");

    const auto submodule = m_parsedData->submodules().at(0).get();
    const auto element = submodule->elements().at(0).get();

    QCOMPARE(element->conflicts().size(), 1);
    QCOMPARE(element->conflicts().at(0).element(), QStringLiteral("ConflictingElement"));
    QCOMPARE(element->conflicts().at(0).submodule(), QStringLiteral("OtherSubmodule"));
    QCOMPARE(element->conflicts().at(0).module(), QStringLiteral("OtherModule"));

    QCOMPARE(element->requirements().size(), 1);
    QCOMPARE(element->requirements().at(0).element(), QStringLiteral("RequiredElement"));
    QCOMPARE(element->requirements().at(0).submodule(), QStringLiteral("LocalSubmodule"));
    QCOMPARE(element->requirements().at(0).module(), QStringLiteral("SomeName"));
}

void ModuleMetadataParserTests::parsingFails(const QString &jsonData)
{
    ModuleMetadataParser parser(jsonData.toUtf8());
    try {
        parser.parse();
        QFAIL("Parsing should fail");
    } catch (const ModuleMetadataParser::Error &err) {
        QVERIFY(QLatin1String(err.what()).size() > 0);
    }
}

void ModuleMetadataParserTests::parse(const QString &jsonData)
{
    ModuleMetadataParser parser(jsonData.toUtf8());
    m_parsedData = parser.parse();
}

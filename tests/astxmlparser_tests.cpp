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
#include "astxmlparser_tests.h"

#include <QtTest>

using namespace Asn1Acn::Internal::Tests;

AstXmlParserTests::AstXmlParserTests(QObject *parent)
    : QObject(parent)
{
}

void AstXmlParserTests::setXmlData(const QString& str)
{
    m_xmlReader.clear();
    m_xmlReader.addData(str);
}

void AstXmlParserTests::test_emptyFile()
{
    parsingFails("");
}

void AstXmlParserTests::test_badXmlRoot()
{
    parsingFails(R"(<?xml version="1.0" encoding="utf-8"?>)"
                 R"(<BadXml></BadXml>)");
}

void AstXmlParserTests::test_missingAsn1Module()
{
    parsingFails(R"(<?xml version="1.0" encoding="utf-8"?>)"
                 R"(<ASN1AST><BadChild/></ASN1AST>)");
}

void AstXmlParserTests::test_emptyDefinitions()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="TestFile.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["TestFile.asn"]->definitions().size(), std::size_t{1});
    QCOMPARE(m_parsedData["TestFile.asn"]->definitions().begin()->first, QStringLiteral("TestDefinitions"));
    QCOMPARE(m_parsedData["TestFile.asn"]->definitions().begin()->second->name(), QStringLiteral("TestDefinitions"));
    QCOMPARE(m_parsedData["TestFile.asn"]->definitions().begin()->second->location().path(), QStringLiteral("TestFile.asn"));
}

void AstXmlParserTests::test_singleTypeAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10"/>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().size(), std::size_t{1});
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").name(), QStringLiteral("MyInt"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").location().path(), QStringLiteral("Test2File.asn"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").location().line(), 4);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").location().column(), 10);
}

void AstXmlParserTests::test_builtinTypeReference()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(          <Type Line="3" CharPositionInLine="19">)"
          R"(            <IntegerType Min="0" Max="1000"/>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().location().column(), 19);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().location().line(), 3);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().type(), Data::TypeReference::DataType::BuiltIn);
}

void AstXmlParserTests::test_userDefinedTypeReference()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(          <Type Line="3" CharPositionInLine="19">)"
          R"(            <ReferenceType ReferencedTypeName="ThirdInt" Min="10" Max="10"/>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().location().column(), 19);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().location().line(), 3);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().type(), Data::TypeReference::DataType::UserDefined);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().name(), QStringLiteral("ThirdInt"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().module(), QStringLiteral("TestDefinitions"));
}

void AstXmlParserTests::test_userDefinedTypeReferenceInOtherModule()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(          <Type Line="3" CharPositionInLine="19">)"
          R"(            <ReferenceType ReferencedTypeName="ThirdInt" Min="10" Max="10" ReferencedModName="MYMOD"/>"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().location().column(), 19);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().location().line(), 3);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().type(), Data::TypeReference::DataType::UserDefined);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().name(), QStringLiteral("ThirdInt"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").reference().module(), QStringLiteral("MYMOD"));
}

void AstXmlParserTests::test_importedTypes()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <ImportedModules>)"
          R"(          <ImportedModule ID="OtherDefinitions">)"
          R"(              <ImportedTypes>)"
          R"(                  <ImportedType Name="Imported" />)"
          R"(              </ImportedTypes>)"
          R"(          </ImportedModule>)"
          R"(      </ImportedModules>)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10"/>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->importedTypes().at(0), QStringLiteral("Imported"));

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().size(), std::size_t{1});
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").name(), QStringLiteral("MyInt"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").location().path(), QStringLiteral("Test2File.asn"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").location().line(), 4);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions().at("TestDefinitions")->types().at("MyInt").location().column(), 10);
}

void AstXmlParserTests::parsingFails(const QString& xmlData)
{
    setXmlData(xmlData);
    AstXmlParser parser(m_xmlReader);
    QVERIFY(!parser.parse());
}

void AstXmlParserTests::parse(const QString& xmlData)
{
    setXmlData(xmlData);
    AstXmlParser parser(m_xmlReader);
    QVERIFY(parser.parse());
    m_parsedData = parser.takeData();
}

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

    QCOMPARE(m_parsedData["TestFile.asn"]->definitionsList().size(), std::size_t{1});
    QCOMPARE(m_parsedData["TestFile.asn"]->definitions("TestDefinitions")->name(), QStringLiteral("TestDefinitions"));
    QCOMPARE(m_parsedData["TestFile.asn"]->definitionsList().front()->name(), QStringLiteral("TestDefinitions"));
    QCOMPARE(m_parsedData["TestFile.asn"]->definitionsList().front()->location().path(), QStringLiteral("TestFile.asn"));

    QCOMPARE(m_parsedData["TestFile.asn"]->definitions("TestDefinitions")->parent(), m_parsedData["TestFile.asn"].get());
}

void AstXmlParserTests::test_singleTypeAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(          <Type>)"
          R"(            <IntegerType />)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->types().size(), std::size_t{1});
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->name(), QStringLiteral("MyInt"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->location().path(), QStringLiteral("Test2File.asn"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->location().line(), 4);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->location().column(), 10);

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->parent(),
             m_parsedData["Test2File.asn"]->definitions("TestDefinitions"));
}

Q_DECLARE_METATYPE(Asn1Acn::Internal::Data::Type)

void AstXmlParserTests::test_builtinTypeReference()
{
    QFETCH(QString, typeName);
    QFETCH(Data::Type, type);

    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(          <Type Line="3" CharPositionInLine="19">)"
          R"(            <)" + typeName + "/>"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().location().column(), 19);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().location().line(), 3);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().type(), type);
}

void AstXmlParserTests::test_builtinTypeReference_data()
{
    QTest::addColumn<QString>("typeName");
    QTest::addColumn<Data::Type>("type");

    QTest::newRow("Boolean")       << "BooleanType"       << Data::Type::Boolean;
    QTest::newRow("Null")          << "NullType"          << Data::Type::Null;
    QTest::newRow("Integer")       << "IntegerType"       << Data::Type::Integer;
    QTest::newRow("Real")          << "RealType"          << Data::Type::Real;
    QTest::newRow("BitString")     << "BitStringType"     << Data::Type::BitString;
    QTest::newRow("OctetString")   << "OctetStringType"   << Data::Type::OctetString;
    QTest::newRow("IA5String")     << "IA5StringType"     << Data::Type::IA5String;
    QTest::newRow("NumericString") << "NumericStringType" << Data::Type::NumericString;
    QTest::newRow("Enumerated")    << "EnumeratedType"    << Data::Type::Enumerated;
    QTest::newRow("Choice")        << "ChoiceType"        << Data::Type::Choice;
    QTest::newRow("Sequence")      << "SequenceType"      << Data::Type::Sequence;
    QTest::newRow("SequenceOf")    << "SequenceOfType"    << Data::Type::SequenceOf;
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

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().location().column(), 19);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().location().line(), 3);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().type(), Data::Type::UserDefined);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().name(), QStringLiteral("ThirdInt"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().module(), QStringLiteral("TestDefinitions"));
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

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().location().column(), 19);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().location().line(), 3);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().type(), Data::Type::UserDefined);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().name(), QStringLiteral("ThirdInt"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().module(), QStringLiteral("MYMOD"));
}

void AstXmlParserTests::test_multipleTypeAssignments()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MySeq" Line="4" CharPositionInLine="10">)"
          R"(          <Type Line="3" CharPositionInLine="19">)"
          R"(            <SequenceType>)"
          R"(              <SequenceOrSetChild/>)"
          R"(            </SequenceType>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(        <TypeAssignment Name="MyNull" Line="4" CharPositionInLine="10">)"
          R"(          <Type Line="3" CharPositionInLine="19">)"
          R"(            <NullType/>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(        <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(          <Type Line="3" CharPositionInLine="19">)"
          R"(            <IntegerType/>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().type(), Data::Type::Integer);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq")->reference().type(), Data::Type::Sequence);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyNull")->reference().type(), Data::Type::Null);
}

void AstXmlParserTests::test_importedType()
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
          R"(      <TypeAssignments/>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->importedTypes().size(), 1);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->importedTypes().at(0), QStringLiteral("Imported"));
}

void AstXmlParserTests::test_multipleImportedType()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <ImportedModules>)"
          R"(          <ImportedModule ID="OtherDefinitions">)"
          R"(              <ImportedTypes>)"
          R"(                  <ImportedType Name="Imported1" />)"
          R"(                  <ImportedType Name="Imported2" />)"
          R"(              </ImportedTypes>)"
          R"(          </ImportedModule>)"
          R"(      </ImportedModules>)"
          R"(      <TypeAssignments/>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->importedTypes().size(), 2);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->importedTypes().at(0), QStringLiteral("Imported1"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->importedTypes().at(1), QStringLiteral("Imported2"));
}

void AstXmlParserTests::test_pathMapping()
{
    m_pathMapper = PathMapper({ {"/xyz/Test2.asn", ""} });
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2.asn">)"
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

    QCOMPARE(m_parsedData["/xyz/Test2.asn"]->definitions("TestDefinitions")->type("MyInt")->reference().location().path(), QStringLiteral("/xyz/Test2.asn"));
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
    AstXmlParser parser(m_xmlReader, m_pathMapper);
    QVERIFY(parser.parse());
    m_parsedData = parser.takeData();
}

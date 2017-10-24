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

#include <data/builtintype.h>

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

void AstXmlParserTests::test_builtinTypeReference()
{
    QFETCH(QString, typeName);
    QFETCH(QString, type);

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

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->type()->name(), type);
}

void AstXmlParserTests::test_builtinTypeReference_data()
{
    QTest::addColumn<QString>("typeName");
    QTest::addColumn<QString>("type");

    QTest::newRow("Boolean")       << "BooleanType"       << "BOOLEAN";
    QTest::newRow("Null")          << "NullType"          << "NULL";
    QTest::newRow("Integer")       << "IntegerType"       << "INTEGER";
    QTest::newRow("Real")          << "RealType"          << "REAL";
    QTest::newRow("BitString")     << "BitStringType"     << "BIT STRING";
    QTest::newRow("OctetString")   << "OctetStringType"   << "OCTET STRING";
    QTest::newRow("IA5String")     << "IA5StringType"     << "IA5String";
    QTest::newRow("NumericString") << "NumericStringType" << "NumericString";
    QTest::newRow("Enumerated")    << "EnumeratedType"    << "ENUMERATED";
    QTest::newRow("Choice")        << "ChoiceType"        << "CHOICE";
    QTest::newRow("Sequence")      << "SequenceType"      << "SEQUENCE";
    QTest::newRow("SequenceOf")    << "SequenceOfType"    << "SEQUENCE OF";
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

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->type()->name(), QStringLiteral("ThirdInt"));

    const auto ref = m_parsedData["Test2File.asn"]->references().find(3);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().column(), 19);
    QCOMPARE(ref->second->location().line(), 3);

    QCOMPARE(ref->second->name(), QStringLiteral("ThirdInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("TestDefinitions"));
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

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->type()->name(), QStringLiteral("ThirdInt"));

    const auto ref = m_parsedData["Test2File.asn"]->references().find(3);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().column(), 19);
    QCOMPARE(ref->second->location().line(), 3);

    QCOMPARE(ref->second->name(), QStringLiteral("ThirdInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("MYMOD"));
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

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt")->type()->name(), QStringLiteral("INTEGER"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq")->type()->name(), QStringLiteral("SEQUENCE"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyNull")->type()->name(), QStringLiteral("NULL"));
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
    m_pathMapper = SourceMapper({ {"/xyz/Test2.asn", ""} });
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

    QCOMPARE(m_parsedData["/xyz/Test2.asn"]->definitions("TestDefinitions")->type("MyInt")->location().path(), QStringLiteral("/xyz/Test2.asn"));
}

void AstXmlParserTests::test_sequenceTypeAssingment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MySeq" Line="19" CharPositionInLine="4">)"
          R"(          <Type Line="11" CharPositionInLine="44">)"
          R"(            <SequenceType>)"
          R"(              <SequenceOrSetChild VarName="applicationProcessId" Optional="False" Line="13" CharPositionInLine="7">)"
          R"(                <Type Line="13" CharPositionInLine="28">)"
          R"(                  <ReferenceType ReferencedTypeName="ApplicationProcess-ID" Min="MIN" Max="MAX" ReferencedModName="ApplicationProcess"/>)"
          R"(                </Type>)"
          R"(              </SequenceOrSetChild>)"
          R"(              <SequenceOrSetChild VarName="applicationProcessesCnt" Optional="False" Line="14" CharPositionInLine="7">)"
          R"(                <Type Line="14" CharPositionInLine="31">)"
          R"(                  <IntegerType Min="MIN" Max="MAX"/>)"
          R"(                </Type>)"
          R"(              </SequenceOrSetChild>)"
          R"(            </SequenceType>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq")->type()->name(), QStringLiteral("SEQUENCE"));

    QVERIFY(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq")->location().isValid());
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq")->location().line(), 19);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq")->location().column(), 4);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeq")->location().path(),
            QLatin1String("Test2File.asn"));

    QCOMPARE(static_cast<int>(m_parsedData["Test2File.asn"]->references().size()), 1);

    const auto ref = m_parsedData["Test2File.asn"]->references().find(13);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 13);
    QCOMPARE(ref->second->location().column(), 28);

    QCOMPARE(ref->second->name(), QStringLiteral("ApplicationProcess-ID"));
    QCOMPARE(ref->second->module(), QStringLiteral("ApplicationProcess"));
}

void AstXmlParserTests::test_sequenceOfTypeAssingment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<ASN1AST>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Asn1Module ID="TestDefinitions">)"
          R"(      <TypeAssignments>)"
          R"(        <TypeAssignment Name="MySeqOf" Line="17" CharPositionInLine="4">)"
          R"(          <Type Line="17" CharPositionInLine="15">)"
          R"(            <SequenceOfType Min="MIN" Max="MAX">)"
          R"(              <Type Line="17" CharPositionInLine="27">)"
          R"(                <ReferenceType ReferencedTypeName="ApplicationProcess-ID" Min="MIN" Max="MAX" ReferencedModName="ApplicationProcess"/>)"
          R"(              </Type>)"
          R"(            </SequenceOfType>)"
          R"(          </Type>)"
          R"(        </TypeAssignment>)"
          R"(      </TypeAssignments>)"
          R"(    </Asn1Module>)"
          R"(  </Asn1File>)"
          R"(</ASN1AST>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeqOf")->type()->name(), QStringLiteral("SEQUENCE OF"));

    QVERIFY(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeqOf")->location().isValid());
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeqOf")->location().line(), 17);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeqOf")->location().column(), 4);
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MySeqOf")->location().path(),
            QLatin1String("Test2File.asn"));

    QCOMPARE(static_cast<int>(m_parsedData["Test2File.asn"]->references().size()), 1);

    const auto ref = m_parsedData["Test2File.asn"]->references().find(17);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 17);
    QCOMPARE(ref->second->location().column(), 27);

    QCOMPARE(ref->second->name(), QStringLiteral("ApplicationProcess-ID"));
    QCOMPARE(ref->second->module(), QStringLiteral("ApplicationProcess"));
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

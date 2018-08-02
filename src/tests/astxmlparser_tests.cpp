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
#include "astxmlparser_tests.h"

#include <QtTest>

#include <data/types/builtintypes.h>

using namespace Asn1Acn::Internal::Tests;

AstXmlParserTests::AstXmlParserTests(QObject *parent)
    : QObject(parent)
{}

void AstXmlParserTests::setXmlData(const QString &str)
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

void AstXmlParserTests::test_emptyDefinitions()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="TestFile.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(  <AcnDependencies/>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["TestFile.asn"]->definitionsList().size(), std::size_t{1});
    const auto defs = m_parsedData["TestFile.asn"]->definitions("TestDefinitions");
    QCOMPARE(defs->name(), QStringLiteral("TestDefinitions"));
    QCOMPARE(defs->location().path(), QStringLiteral("TestFile.asn"));
    QCOMPARE(defs->location().line(), 13);
    QCOMPARE(defs->location().column(), 42);

    QCOMPARE(defs->parent(), m_parsedData["TestFile.asn"].get());
}

void AstXmlParserTests::test_singleTypeAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(            <Asn1Type>)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->types().size(),
             std::size_t{1});
    const auto type = m_parsedData["Test2File.asn"]->definitions("TestDefinitions")->type("MyInt");
    QCOMPARE(type->name(), QStringLiteral("MyInt"));
    QCOMPARE(type->location().path(), QStringLiteral("Test2File.asn"));
    QCOMPARE(type->location().line(), 4);
    QCOMPARE(type->location().column(), 10);

    QCOMPARE(type->parent(), m_parsedData["Test2File.asn"]->definitions("TestDefinitions"));
}

void AstXmlParserTests::test_builtinTypeReference()
{
    QFETCH(QString, xmlTypeName);
    QFETCH(QString, typeName);

    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="4" CharPositionInLine="10">)"
          R"(            <Asn1Type>)"
          R"(              <)"
          + xmlTypeName
          + "/>"
            R"(            </Asn1Type>)"
            R"(          </TypeAssignment>)"
            R"(        </TypeAssignments>)"
            R"(      </Module>)"
            R"(    </Modules>)"
            R"(  </Asn1File>)"
            R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->name(), typeName);
}

void AstXmlParserTests::test_builtinTypeReference_data()
{
    QTest::addColumn<QString>("xmlTypeName");
    QTest::addColumn<QString>("typeName");

    // clang-format off
    QTest::newRow("Boolean")       << "BOOLEAN"       << "BOOLEAN";
    QTest::newRow("Null")          << "NULL"          << "NULL";
    QTest::newRow("Integer")       << "INTEGER"       << "INTEGER";
    QTest::newRow("Real")          << "REAL"          << "REAL";
    QTest::newRow("BitString")     << "BIT_STRING"    << "BIT STRING";
    QTest::newRow("OctetString")   << "OCTET_STRING"  << "OCTET STRING";
    QTest::newRow("IA5String")     << "IA5String"     << "IA5String";
    QTest::newRow("NumericString") << "NumericString" << "NumericString";
    QTest::newRow("Enumerated")    << "ENUMERATED"    << "ENUMERATED";
    QTest::newRow("Choice")        << "CHOICE"        << "CHOICE";
    QTest::newRow("Sequence")      << "SEQUENCE"      << "SEQUENCE";
    QTest::newRow("SequenceOf")    << "SEQUENCE_OF"   << "SEQUENCE OF";
    // clang-format on
}

void AstXmlParserTests::test_userDefinedTypeReference()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <REFERENCE_TYPE Module="OtherModule" TypeAssignment="OrgInt"/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->name(),
             QStringLiteral("OrgInt"));

    const auto ref = m_parsedData["X.asn"]->referencesMap().find(3);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().column(), 19);
    QCOMPARE(ref->second->location().line(), 3);

    QCOMPARE(ref->second->name(), QStringLiteral("OrgInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("OtherModule"));
}

void AstXmlParserTests::test_multipleTypeAssignments()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <INTEGER/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(          <TypeAssignment Name="MyBool" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <BOOLEAN/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(          <TypeAssignment Name="MyNull" Line="2" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <NULL/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyInt")->type()->name(),
             QStringLiteral("INTEGER"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyBool")->type()->name(),
             QStringLiteral("BOOLEAN"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->type("MyNull")->type()->name(),
             QStringLiteral("NULL"));
}

void AstXmlParserTests::test_importedType()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedTypes>)"
          R"(                    <ImportedType Name="Imported" />)"
          R"(                </ImportedTypes>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(        <TypeAssignments/>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().size(), std::size_t(1));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).module(),
             QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).name(),
             QStringLiteral("Imported"));
}

void AstXmlParserTests::test_multipleImportedType()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="X.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedTypes>)"
          R"(                    <ImportedType Name="Imported1" />)"
          R"(                    <ImportedType Name="Imported2" />)"
          R"(                </ImportedTypes>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(        <TypeAssignments/>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().size(), std::size_t(2));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(0).name(),
             QStringLiteral("Imported1"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(1).name(),
             QStringLiteral("Imported2"));
    QCOMPARE(m_parsedData["X.asn"]->definitions("Defs")->importedTypes().at(1).module(),
             QStringLiteral("OtherDefinitions"));
}

void AstXmlParserTests::test_assignmentsAreTypeReferences()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="TestDefinitions" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeq" Line="19" CharPositionInLine="4">)"
          R"(            <Asn1Type Line="3" CharPositionInLine="19">)"
          R"(              <INTEGER/>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2.asn"]->referencesMap().size(), std::size_t{1});

    const auto ref = m_parsedData["Test2.asn"]->referencesMap().find(19);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 19);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MySeq"));
    QCOMPARE(ref->second->module(), QStringLiteral("TestDefinitions"));
}

void AstXmlParserTests::test_sequenceTypeAssingment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeq" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <SEQUENCE>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <SEQUENCE_COMPONENT Name="a" Line="13" CharPositionInLine="3">)"
          R"(                  <Asn1Type Line="13" CharPositionInLine="8">)"
          R"(                    <INTEGER/>)"
          R"(                  </Asn1Type>)"
          R"(                </SEQUENCE_COMPONENT>)"
          R"(                <SEQUENCE_COMPONENT Name="b" Line="14" CharPositionInLine="2">)"
          R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
          R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                  </Asn1Type>)"
          R"(                </SEQUENCE_COMPONENT>)"
          R"(              </SEQUENCE>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeq");
    QCOMPARE(seq->type()->name(), QStringLiteral("SEQUENCE"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t{2});

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 14);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_choiceTypeAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyChoice" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <CHOICE>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <CHOICE_ALTERNATIVE Name="a" Line="13" CharPositionInLine="3">)"
          R"(                  <Asn1Type Line="13" CharPositionInLine="5">)"
          R"(                    <INTEGER/>)"
          R"(                  </Asn1Type>)"
          R"(                </CHOICE_ALTERNATIVE>)"
          R"(                <CHOICE_ALTERNATIVE Name="b" Line="14" CharPositionInLine="2">)"
          R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
          R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                  </Asn1Type>)"
          R"(                </CHOICE_ALTERNATIVE>)"
          R"(              </CHOICE>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto choice = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MyChoice");
    QCOMPARE(choice->type()->name(), QStringLiteral("CHOICE"));

    QCOMPARE(choice->location().line(), 11);
    QCOMPARE(choice->location().column(), 9);
    QCOMPARE(choice->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t{2});

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 14);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_sequenceOfTypeAssingment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MySeqOf" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <SEQUENCE_OF>)"
          R"(                <Constraints/>)"
          R"(                <WithComponentConstraints/>)"
          R"(                <Asn1Type Line="13" CharPositionInLine="4">)"
          R"(                  <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
          R"(                </Asn1Type>)"
          R"(              </SEQUENCE_OF>"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeqOf");
    QCOMPARE(seq->type()->name(), QStringLiteral("SEQUENCE OF"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t{2});

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(13);
    QVERIFY(ref->second != nullptr);

    QCOMPARE(ref->second->location().line(), 13);
    QCOMPARE(ref->second->location().column(), 4);

    QCOMPARE(ref->second->name(), QStringLiteral("MyInt"));
    QCOMPARE(ref->second->module(), QStringLiteral("Other"));
}

void AstXmlParserTests::test_valueAssignment()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ValueAssignments>)"
          R"(          <ValueAssignment Name="asnConstant" Line="2" CharPositionInLine="0">)"
          R"(            <Asn1Type Line="2" CharPositionInLine="6">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </ValueAssignment>)"
          R"(        </ValueAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->values().size(), std::size_t{1});
    const auto var = m_parsedData["Test2File.asn"]->definitions("Defs")->value("asnConstant");
    QCOMPARE(var->name(), QStringLiteral("asnConstant"));
    QCOMPARE(var->location().line(), 2);
    QCOMPARE(var->type()->name(), QStringLiteral("INTEGER"));
}

void AstXmlParserTests::test_importedValues()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedValues>)"
          R"(                    <ImportedValue Name="Imported" />)"
          R"(                </ImportedValues>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().size(),
             std::size_t(1));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).module(),
             QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).name(),
             QStringLiteral("Imported"));
}

void AstXmlParserTests::test_multipleImportedValues()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="Test2File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <ImportedModules>)"
          R"(            <ImportedModule ID="OtherDefinitions">)"
          R"(                <ImportedValues>)"
          R"(                    <ImportedValue Name="Imported" />)"
          R"(                    <ImportedValue Name="SecondImported" />)"
          R"(                </ImportedValues>)"
          R"(            </ImportedModule>)"
          R"(        </ImportedModules>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().size(),
             std::size_t(2));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).module(),
             QStringLiteral("OtherDefinitions"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(0).name(),
             QStringLiteral("Imported"));
    QCOMPARE(m_parsedData["Test2File.asn"]->definitions("Defs")->importedValues().at(1).name(),
             QStringLiteral("SecondImported"));
}

void AstXmlParserTests::test_multipleModules()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(      <Module Name="Defs2" Line="42" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt2" Line="99" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs")->type("MyInt")->location().line(), 11);
    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs2")->type("MyInt2")->location().line(), 99);
}

void AstXmlParserTests::test_multipleFiles()
{
    parse(R"(<?xml version="1.0" encoding="utf-8"?>)"
          R"(<AstRoot>)"
          R"(  <Asn1File FileName="File.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt" Line="11" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(  <Asn1File FileName="File2.asn">)"
          R"(    <Modules>)"
          R"(      <Module Name="Defs2" Line="4" CharPositionInLine="42">)"
          R"(        <TypeAssignments>)"
          R"(          <TypeAssignment Name="MyInt2" Line="9" CharPositionInLine="9">)"
          R"(            <Asn1Type Line="12" CharPositionInLine="19">)"
          R"(              <INTEGER/>)"
          R"(            </Asn1Type>)"
          R"(          </TypeAssignment>)"
          R"(        </TypeAssignments>)"
          R"(      </Module>)"
          R"(    </Modules>)"
          R"(  </Asn1File>)"
          R"(</AstRoot>)");

    QCOMPARE(m_parsedData["File.asn"]->definitions("Defs")->type("MyInt")->location().line(), 11);
    QCOMPARE(m_parsedData["File2.asn"]->definitions("Defs2")->type("MyInt2")->location().line(), 9);
}

void AstXmlParserTests::test_parametrizedInstancesContentsAreIgnored()
{
    parse(
        R"(<?xml version="1.0" encoding="utf-8"?>)"
        R"(<AstRoot>)"
        R"(  <Asn1File FileName="Test2File.asn">)"
        R"(    <Modules>)"
        R"(      <Module Name="Defs" Line="13" CharPositionInLine="42">)"
        R"(        <TypeAssignments>)"
        R"(          <TypeAssignment Name="MySeq" Line="11" CharPositionInLine="9">)"
        R"(            <Asn1Type Line="12" CharPositionInLine="19" ParameterizedTypeInstance="true">)"
        R"(              <SEQUENCE>)"
        R"(                <Constraints/>)"
        R"(                <WithComponentConstraints/>)"
        R"(                <SEQUENCE_COMPONENT Name="b" Line="14" CharPositionInLine="2">)"
        R"(                  <Asn1Type Line="14" CharPositionInLine="4">)"
        R"(                    <REFERENCE_TYPE Module="Other" TypeAssignment="MyInt"/>")
        R"(                  </Asn1Type>)"
        R"(                </SEQUENCE_COMPONENT>)"
        R"(              </SEQUENCE>)"
        R"(            </Asn1Type>)"
        R"(          </TypeAssignment>)"
        R"(        </TypeAssignments>)"
        R"(      </Module>)"
        R"(    </Modules>)"
        R"(  </Asn1File>)"
        R"(</AstRoot>)");

    const auto seq = m_parsedData["Test2File.asn"]->definitions("Defs")->type("MySeq");
    QCOMPARE(seq->type()->name(), QStringLiteral("SEQUENCE"));

    QCOMPARE(seq->location().line(), 11);
    QCOMPARE(seq->location().column(), 9);
    QCOMPARE(seq->location().path(), QLatin1String("Test2File.asn"));

    QCOMPARE(m_parsedData["Test2File.asn"]->referencesMap().size(), std::size_t{1});

    const auto ref = m_parsedData["Test2File.asn"]->referencesMap().find(14);
    QVERIFY(ref == m_parsedData["Test2File.asn"]->referencesMap().end());
}

void AstXmlParserTests::parsingFails(const QString &xmlData)
{
    setXmlData(xmlData);
    AstXmlParser parser(m_xmlReader);
    QVERIFY(!parser.parse());
}

void AstXmlParserTests::parse(const QString &xmlData)
{
    setXmlData(xmlData);
    AstXmlParser parser(m_xmlReader);
    QVERIFY(parser.parse());
    m_parsedData = parser.takeData();
}

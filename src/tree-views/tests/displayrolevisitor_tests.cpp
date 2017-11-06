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
#include "displayrolevisitor_tests.h"

#include <QtTest>

#include <data/types/builtintypes.h>
#include <data/types/userdefinedtype.h>
#include <data/typereference.h>
#include <data/definitions.h>
#include <data/typeassignment.h>
#include <data/file.h>

using namespace Asn1Acn::Internal::TreeViews::Tests;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::Data;

DisplayRoleVisitorTests::DisplayRoleVisitorTests(QObject *parent)
    : QObject(parent)
{
}

void DisplayRoleVisitorTests::test_definitions()
{
    Definitions defs("DefName", {});

    QCOMPARE(defs.valueFor<DisplayRoleVisitor>(), QStringLiteral("DefName"));
}

void DisplayRoleVisitorTests::test_file()
{
    File file("File");

    QCOMPARE(file.valueFor<DisplayRoleVisitor>(), QStringLiteral("File"));
}

void DisplayRoleVisitorTests::test_typeAssignmentBuiltIn()
{
    QFETCH(QString, typeName);
    QFETCH(QString, type);

    TypeAssignment typeAssignment("TypeName", {}, Data::Types::BuiltinType::createBuiltinType(typeName));

    QCOMPARE(typeAssignment.valueFor<DisplayRoleVisitor>(), QString("TypeName: " + type));
}

void DisplayRoleVisitorTests::test_typeAssignmentBuiltIn_data()
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

void DisplayRoleVisitorTests::test_typeAssignmentUserDefined()
{
    TypeAssignment typeAssignment("TypeName", {}, std::make_unique<Data::Types::UserdefinedType>("type", "module"));
    QCOMPARE(typeAssignment.valueFor<DisplayRoleVisitor>(), QString("TypeName: type.module"));
}

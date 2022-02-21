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
#include "displayrolevisitor_tests.h"

#include <QtTest>

#include <data/definitions.h>
#include <data/file.h>
#include <data/typeassignment.h>
#include <data/typereference.h>
#include <data/types/builtintypes.h>
#include <data/types/userdefinedtype.h>

using namespace Asn1Acn::Internal::TreeViews::Tests;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::Data;

DisplayRoleVisitorTests::DisplayRoleVisitorTests(QObject *parent)
    : QObject(parent)
{}

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
    QFETCH(QString, typeDisplayName);

    TypeAssignment typeAssignment("TypeName",
                                  {},
                                  Data::Types::BuiltinType::createBuiltinType(typeName));

    QCOMPARE(typeAssignment.valueFor<DisplayRoleVisitor>(), QString("TypeName: " + typeDisplayName));
}

void DisplayRoleVisitorTests::test_typeAssignmentBuiltIn_data()
{
    QTest::addColumn<QString>("typeName");
    QTest::addColumn<QString>("typeDisplayName");

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

void DisplayRoleVisitorTests::test_typeAssignmentUserDefined()
{
    TypeAssignment typeAssignment("TypeName",
                                  {},
                                  std::make_unique<Data::Types::UserdefinedType>("type", "module"));
    QCOMPARE(typeAssignment.valueFor<DisplayRoleVisitor>(), QString("TypeName: module.type"));
}

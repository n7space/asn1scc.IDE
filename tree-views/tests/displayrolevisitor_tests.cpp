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

#include <data/type.h>
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

Q_DECLARE_METATYPE(Asn1Acn::Internal::Data::Type::Kind)

void DisplayRoleVisitorTests::test_definitions()
{
    Definitions defs("DefName", {});

    QCOMPARE(defs.valueFor<DisplayRoleVisitor>(), QStringLiteral("DefName"));
}

void DisplayRoleVisitorTests::test_file()
{
    File file({"File", ""});

    QCOMPARE(file.valueFor<DisplayRoleVisitor>(), QStringLiteral("File"));
}

void DisplayRoleVisitorTests::test_typeAssignment()
{
    QFETCH(Type::Kind, kind);
    QFETCH(QString, value);

    TypeAssignment typeAssignment("TypeName", {}, Type(kind));


    QCOMPARE(typeAssignment.valueFor<DisplayRoleVisitor>(), QString("TypeName: " + value));
}

void DisplayRoleVisitorTests::test_typeAssignment_data()
{
    QTest::addColumn<Type::Kind>("kind");
    QTest::addColumn<QString>("value");

    QTest::newRow("Boolean")       << Type::Kind::Boolean       << "BOOLEAN";
    QTest::newRow("Null")          << Type::Kind::Null          << "NULL";
    QTest::newRow("Integer")       << Type::Kind::Integer       << "INTEGER";
    QTest::newRow("Real")          << Type::Kind::Real          << "REAL";
    QTest::newRow("BitString")     << Type::Kind::BitString     << "BIT STRING";
    QTest::newRow("OctetString")   << Type::Kind::OctetString   << "OCTET STRING";
    QTest::newRow("IA5String")     << Type::Kind::IA5String     << "IA5String";
    QTest::newRow("NumericString") << Type::Kind::NumericString << "NumericString";
    QTest::newRow("Enumerated")    << Type::Kind::Enumerated    << "ENUMERATED";
    QTest::newRow("Choice")        << Type::Kind::Choice        << "CHOICE";
    QTest::newRow("Sequence")      << Type::Kind::Sequence      << "SEQUENCE";
    QTest::newRow("SequenceOf")    << Type::Kind::SequenceOf    << "SEQUENCE OF";
}

void DisplayRoleVisitorTests::test_typeReferenceBuiltIn()
{
    TypeReference reference(Type::Kind::SequenceOf, {});

    QCOMPARE(reference.valueFor<DisplayRoleVisitor>(), QString());
}

void DisplayRoleVisitorTests::test_typeReferenceUserDefined()
{
    TypeReference reference("Name", "Module", {});

    QCOMPARE(reference.valueFor<DisplayRoleVisitor>(), QString());
}

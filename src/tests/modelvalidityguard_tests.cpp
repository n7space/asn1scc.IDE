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

#include "modelvalidityguard_tests.h"

#include <QtTest>

using namespace Asn1Acn::Internal::Tests;
using namespace Asn1Acn::Internal;

ModelValidityGuardTests::ModelValidityGuardTests(QObject *parent)
    : QObject(parent)
    , m_guard(new ModelValidityGuard)
{}

ModelValidityGuardTests::~ModelValidityGuardTests()
{
    delete m_guard;
}

void ModelValidityGuardTests::test_noModifiers()
{
    QVERIFY(m_guard->isValid());
}

void ModelValidityGuardTests::test_singleModifiers()
{
    QSignalSpy spyAboutToUpdate(m_guard, &ModelValidityGuard::modelAboutToChange);
    QSignalSpy spyUpdated(m_guard, &ModelValidityGuard::modelChanged);

    m_guard->invalidate();
    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(m_guard->isValid(), false);

    m_guard->validate();
    QCOMPARE(spyAboutToUpdate.count(), 1);
    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_guard->isValid(), true);
}

void ModelValidityGuardTests::test_multipleModifiers()
{
    QSignalSpy spyAboutToUpdate(m_guard, &ModelValidityGuard::modelAboutToChange);
    QSignalSpy spyUpdated(m_guard, &ModelValidityGuard::modelChanged);

    const int modifiersCnt = 5;

    for (int i = 0; i < modifiersCnt; i++)
        m_guard->invalidate();

    QCOMPARE(spyAboutToUpdate.count(), modifiersCnt);
    QCOMPARE(m_guard->isValid(), false);

    for (int i = 0; i < modifiersCnt; i++)
        m_guard->validate();

    QCOMPARE(spyAboutToUpdate.count(), modifiersCnt);
    QCOMPARE(spyUpdated.count(), 1);
    QCOMPARE(m_guard->isValid(), true);
}

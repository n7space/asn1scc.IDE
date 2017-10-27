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
#include "outlinemodel_tests.h"

#include "../outlinemodel.h"

#include <data/file.h>
#include <data/definitions.h>
#include <data/source.h>
#include <data/typeassignment.h>
#include <data/builtintypes.h>

#include "3rdparty/modeltest.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::TreeViews::Tests;

OutlineModelTests::OutlineModelTests(QObject *parent)
    : QObject(parent)
{
}

OutlineModelTests::~OutlineModelTests()
{
}

void OutlineModelTests::test_emptyModel()
{
    ModelTest testEmptyModel(new OutlineModel(this), this);
}

void OutlineModelTests::test_modelWithDummyPopulation()
{
    const auto root = std::make_unique<Data::File>(Data::Source("file.asn1", "CONTENTS"));

    auto definitions1 = std::make_unique<Data::Definitions>("Module1", Data::SourceLocation{"file1.asn1", 0, 0});
    definitions1->add(std::make_unique<Data::TypeAssignment>("Num1", Data::SourceLocation{"file1.asn1", 2, 3}, Data::BuiltinType::createBuiltinType("IntegerType")));
    definitions1->add(std::make_unique<Data::TypeAssignment>("Num2", Data::SourceLocation{"file1.asn1", 3, 3}, Data::BuiltinType::createBuiltinType("IntegerType")));
    root->add(std::move(definitions1));

    auto definitions2 = std::make_unique<Data::Definitions>("Module2", Data::SourceLocation{"file1.asn1", 5, 0});
    definitions2->add(std::make_unique<Data::TypeAssignment>("Num3", Data::SourceLocation{"file1.asn1", 6, 3}, Data::BuiltinType::createBuiltinType("IntegerType")));
    definitions2->add(std::make_unique<Data::TypeAssignment>("Num4", Data::SourceLocation{"file1.asn1", 7, 3}, Data::BuiltinType::createBuiltinType("IntegerType")));
    root->add(std::move(definitions2));

    auto model = new OutlineModel(this);
    model->setRoot(root.get());

    ModelTest testNonEmptyModel(model, this);
}

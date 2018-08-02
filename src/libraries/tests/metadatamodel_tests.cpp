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

#include "metadatamodel_tests.h"

#include <memory>

#include <libraries/metadatamodel.h>

#include <libraries/metadata/element.h>
#include <libraries/metadata/library.h>
#include <libraries/metadata/module.h>
#include <libraries/metadata/submodule.h>

#include <3rdparty/tests/modeltest.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Tests;

MetadataModelTests::MetadataModelTests(QObject *parent)
    : QObject(parent)
{}

void MetadataModelTests::test_emptyModel()
{
    Metadata::Library library(QLatin1String("Library"));

    const auto model = std::make_unique<MetadataModel>(&library, this);
    ModelTest testEmptyModel(model.get(), this);
}

void MetadataModelTests::test_modelWithDummyPopulation()
{
    auto element1 = std::make_unique<Metadata::Element>(QLatin1String("Element_1_Name"),
                                                        QLatin1String("Element_1_Description"));
    auto element2 = std::make_unique<Metadata::Element>(QLatin1String("Element_2_Name"),
                                                        QLatin1String("Element_2_Description"));
    auto submodule1 = std::make_unique<Metadata::Submodule>(QLatin1String("Submodule_1_Name"),
                                                            QLatin1String(
                                                                "Submodule_1_Description"));
    submodule1->addElement(std::move(element1));
    submodule1->addElement(std::move(element2));

    auto element3 = std::make_unique<Metadata::Element>(QLatin1String("Element_3_Name"),
                                                        QLatin1String("Element_3_Description"));
    auto element4 = std::make_unique<Metadata::Element>(QLatin1String("Element_4_Name"),
                                                        QLatin1String("Element_4_Description"));
    auto submodule2 = std::make_unique<Metadata::Submodule>(QLatin1String("Submodule_2_Name"),
                                                            QLatin1String(
                                                                "Submodule_2_Description"));
    submodule2->addElement(std::move(element3));
    submodule2->addElement(std::move(element4));

    auto module1 = std::make_unique<Metadata::Module>(QLatin1String("Module_1_Name"),
                                                      QLatin1String("Module_1_Description"));
    module1->addSubmodule(std::move(submodule1));
    auto module2 = std::make_unique<Metadata::Module>(QLatin1String("Module_2_Name"),
                                                      QLatin1String("Module_2_Description"));
    module2->addSubmodule(std::move(submodule2));

    auto library = std::make_unique<Metadata::Library>(QLatin1String("LibraryName"));
    library->addModule(std::move(module1));
    library->addModule(std::move(module2));

    const auto model = std::make_unique<MetadataModel>(library.get(), this);
    ModelTest testNonEmptyModel(model.get(), this);
}

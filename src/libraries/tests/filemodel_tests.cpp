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

#include "filemodel_tests.h"

#include <memory>

#include <QDir>
#include <QFileSystemModel>

#include <libraries/filemodel.h>

#include <3rdparty/tests/modeltest.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Tests;

FileModelTests::FileModelTests(QObject *parent)
    : QObject(parent)
{}

void FileModelTests::test_emptyModel()
{
    const auto model = std::make_unique<FileModel>(this);
    ModelTest testEmptyModel(model.get(), this);
}

void FileModelTests::test_modelWithDummyPopulation()
{
    const auto model = std::make_unique<FileModel>(this);
    model->setRootPath(QDir::currentPath());

    ModelTest testNonEmptyModel(model.get(), this);
}

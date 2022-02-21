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
#pragma once

#include <QObject>

#include "../modulemetadataparser.h"

#include <libraries/metadata/module.h>

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Tests {

class ModuleMetadataParserTests : public QObject
{
    Q_OBJECT
public:
    explicit ModuleMetadataParserTests(QObject *parent = 0);

private slots:
    void test_emptyFile();
    void test_malformedJson();
    void test_wrongJsonType();
    void test_emptyObject();
    void test_emptyModule();
    void test_emptySubmodule();
    void test_emptyElement();
    void test_completeElement();
    void test_complexReferences();

private:
    void parsingFails(const QString &jsonData);
    void parse(const QString &jsonData);

    std::unique_ptr<Metadata::Module> m_parsedData;
};

} // namespace Tests
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

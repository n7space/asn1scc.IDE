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

#pragma once

#include <QObject>

#include "../astxmlparser.h"

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class AstXmlParserTests : public QObject
{
    Q_OBJECT
public:
    explicit AstXmlParserTests(QObject *parent = 0);

private slots:
    void test_emptyFile();
    void test_badXmlRoot();
    void test_missingAsn1Module();
    void test_emptyDefinitions();
    void test_singleTypeAssignment();
    void test_builtinTypeReference();
    void test_builtinTypeReference_data();
    void test_userDefinedTypeReference();
    void test_userDefinedTypeReferenceInOtherModule();
    void test_multipleTypeAssignments();
    void test_importedType();
    void test_multipleImportedType();
    void test_pathMapping();
    void test_assignmentsAreTypeReferences();
    void test_sequenceTypeAssingment();
    void test_sequenceOfTypeAssingment();
    void test_choiceTypeAssignment();
    void test_variableAssignment();
    void test_importedVariable();
    void test_multipleImportedVariable();

private:
    void setXmlData(const QString& str);
    void parsingFails(const QString& xmlData);
    void parse(const QString& xmlData);

    QXmlStreamReader m_xmlReader;
    SourceMapper m_pathMapper;
    std::map<QString, std::unique_ptr<Data::File>> m_parsedData;
};

} // namespace Tests
} // namespace Internal
} // namespace Asn1Acn

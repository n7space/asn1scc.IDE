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

#include <memory>

#include <QXmlStreamReader>

#include <QString>

#include "data/modules.h"

namespace Asn1Acn {
namespace Internal {

class AstXmlParser
{
public:
    AstXmlParser(QXmlStreamReader &m_xmlReader);

    bool parse();

    std::map<QString, std::unique_ptr<Data::Modules>> takeData() { return std::move(m_data); }

private:
    void readASN1AST();
    void readAsn1File();
    void readAsn1Module();
    void readExportedTypes();
    void readExportedVariables();
    void readImportedModules();
    void readTypeAssignments();
    void readVariablesAssignments();
    void readAsn1ModuleChildren();
    void readTypeAssignment();

    void updateCurrentFile();
    void createNewAsn1Module();
    QString readAsnModuleId();
    QString readNameAttribute();
    int readLineAttribute();
    int readCharPossitionInLineAttribute();

    void readImportedModule();
    void readImportedVariables();
    void readImportedTypes();
    void readImportedType();

    Data::SourceLocation readLocationFromAttributes();

    Data::TypeReference readTypeReference();
    Data::TypeReference readType(const Data::SourceLocation &location);
    QString readReferencedTypeNameAttribute();
    QString readReferencedModuleAttribute();

    bool nextRequiredElementIs(const QString &name);

    QXmlStreamReader& m_xmlReader;
    std::map<QString, std::unique_ptr<Data::Modules>> m_data;
    QString m_currentFile;
    QString m_currentModule;
    Data::Definitions* m_currentDefinitions;
};

} // namespace Internal
} // namespace Asn1Acn

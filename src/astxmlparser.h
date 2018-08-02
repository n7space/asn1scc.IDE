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
#pragma once

#include <memory>

#include <QXmlStreamReader>

#include <QString>

#include "data/file.h"

namespace Asn1Acn {
namespace Internal {

class AstXmlParser
{
public:
    explicit AstXmlParser(QXmlStreamReader &xmlReader);

    bool parse();

    std::map<QString, std::unique_ptr<Data::File>> takeData() { return std::move(m_data); }

private:
    void readAstRoot();
    void readAsn1File();
    void readModules();
    void readModule();
    void readExportedTypes();
    void readExportedValues();
    void readImportedModules();
    void readTypeAssignments();
    void readValueAssignments();
    void readModuleChildren();
    void readTypeAssignment();
    void readValueAssignment();

    void updateCurrentFile();
    void createNewModule();
    QString readIdAttribute();
    QString readNameAttribute();
    int readLineAttribute();
    int readCharPossitionInLineAttribute();
    bool isParametrizedTypeInstance() const;

    void readImportedModule();
    void readImportedValues(const QString &moduleName);
    void readImportedValue(const QString &moduleName);
    void readImportedTypes(const QString &moduleName);
    void readImportedType(const QString &moduleName);

    Data::SourceLocation readLocationFromAttributes();

    std::unique_ptr<Data::Types::Type> readType();
    std::unique_ptr<Data::Types::Type> readTypeDetails(const Data::SourceLocation &location);
    std::unique_ptr<Data::Types::Type> readReferenceType(const Data::SourceLocation &location);
    std::unique_ptr<Data::Types::Type> buildTypeFromName(const Data::SourceLocation &location,
                                                         const QStringRef &name);

    void readTypeContents(const QStringRef &name);
    void readSequence();
    void readSequenceOf();
    void readChoice();

    QString readTypeAssignmentAttribute();
    QString readModuleAttribute();

    bool nextRequiredElementIs(const QString &name);
    bool skipToChildElement(const QString &name);

    QXmlStreamReader &m_xmlReader;
    std::map<QString, std::unique_ptr<Data::File>> m_data;
    QString m_currentFile;
    QString m_currentModule;
    Data::Definitions *m_currentDefinitions;
};

} // namespace Internal
} // namespace Asn1Acn

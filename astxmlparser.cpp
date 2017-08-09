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
#include "astxmlparser.h"

using namespace Asn1Acn::Internal;

AstXmlParser::AstXmlParser(QXmlStreamReader& xmlReader_)
    : m_xmlReader(xmlReader_),
      m_currentDefinitions(nullptr)
{
}

bool AstXmlParser::parse()
{
    if (nextRequiredElementIs(QStringLiteral("ASN1AST")))
            readASN1AST();
    return !m_xmlReader.hasError();
}

void AstXmlParser::readASN1AST()
{
    while (nextRequiredElementIs(QStringLiteral("Asn1File")))
        readAsn1File();
}

void AstXmlParser::readAsn1File()
{
    updateCurrentFile();
    while (nextRequiredElementIs(QStringLiteral("Asn1Module")))
        readAsn1Module();
}

void AstXmlParser::updateCurrentFile()
{
    m_currentFile = m_xmlReader.attributes().value(QStringLiteral("FileName")).toString();

    m_data.insert(std::make_pair(m_currentFile, std::make_unique<Data::Modules>()));
}

void AstXmlParser::readAsn1ModuleChildren()
{
    while (m_xmlReader.readNextStartElement())
    {
        if (m_xmlReader.name() == QStringLiteral("ExportedTypes"))
            readExportedTypes();
        else if (m_xmlReader.name() == QStringLiteral("ExportedVariables"))
            readExportedVariables();
        else if (m_xmlReader.name() == QStringLiteral("ImportedModules"))
            readImportedModules();
        else if (m_xmlReader.name() == QStringLiteral("TypeAssignments"))
            readTypeAssignments();
        else if (m_xmlReader.name() == QStringLiteral("VariablesAssignments"))
            readVariablesAssignments();
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::createNewAsn1Module()
{
    const auto location = Data::SourceLocation(m_currentFile, 0, 0); // TODO location of DEFINITIONS begin (fix in asn1scc required)
    m_currentModule = readAsnModuleId();
    auto module = std::unique_ptr<Data::Definitions>(new Data::Definitions(m_currentModule, location));
    m_currentDefinitions = module.get();
    m_data[m_currentFile]->add(std::move(module));
}

void AstXmlParser::readAsn1Module()
{
    createNewAsn1Module();
    readAsn1ModuleChildren();
}

QString AstXmlParser::readAsnModuleId()
{
    return m_xmlReader.attributes().value("ID").toString();
}

void AstXmlParser::readVariablesAssignments()
{
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readTypeAssignments()
{
    while (nextRequiredElementIs(QStringLiteral("TypeAssignment")))
        readTypeAssignment();
}

void AstXmlParser::readTypeAssignment()
{
    Q_ASSERT(m_currentDefinitions != nullptr);
    const auto location = Data::SourceLocation(m_currentFile, readLineAttribute(), readCharPossitionInLineAttribute());
    const auto name = readNameAttribute();

    readTypeAssignmentAttributes();
    const auto type = Data::TypeAssignment(name, location, m_typeReference);
    m_typeReference = Data::TypeReference();

    m_currentDefinitions->add(type);
}

void AstXmlParser::readTypeAssignmentAttributes()
{
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == QStringLiteral("Type")) {
            readTypeAssignmentType();
            m_xmlReader.skipCurrentElement();
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void AstXmlParser::readTypeAssignmentType()
{
    const int line = readLineAttribute();
    const int positionInLine = readCharPossitionInLineAttribute();

    m_xmlReader.readNextStartElement();

    const QStringRef attributeName = m_xmlReader.name();
    if (attributeName == QStringLiteral("ReferenceType"))
        readTypeAssignmentReferencedType(line, positionInLine);
    else
        readTypeAssignmentBuiltinType(line, positionInLine);

    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readTypeAssignmentReferencedType(int line, int positionInLine)
{
    QString referenceTypeName = m_xmlReader.attributes().value(QStringLiteral("ReferencedTypeName")).toString();
    QString moduleName = m_xmlReader.attributes().value(QStringLiteral("ReferencedModName")).toString();
    if (moduleName.isEmpty())
        moduleName = m_currentModule;

    Data::TypeReference::DataType type = Data::TypeReference::DataType::UserDefined;

    setTypeAssignmentTypeReference(referenceTypeName, moduleName, line, positionInLine, type);
}

void AstXmlParser::readTypeAssignmentBuiltinType(int line, int positionInLine)
{
    QString referenceTypeName = m_xmlReader.name().toString();
    QString moduleName = m_currentModule;
    Data::TypeReference::DataType type = Data::TypeReference::DataType::BuiltIn;

    setTypeAssignmentTypeReference(referenceTypeName, moduleName, line, positionInLine, type);
}

void AstXmlParser::setTypeAssignmentTypeReference(const QString &referenceTypeName,
                                                  const QString &moduleName,
                                                  int line,
                                                  int positionInLine,
                                                  Data::TypeReference::DataType type)
{
    m_typeReference = Data::TypeReference(referenceTypeName,
                                          Data::SourceLocation(m_currentFile, line, positionInLine),
                                          type,
                                          moduleName);
}

QString AstXmlParser::readNameAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Name")).toString();
}

int AstXmlParser::readLineAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Line")).toInt();
}

int AstXmlParser::readCharPossitionInLineAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("CharPositionInLine")).toInt();
}

void AstXmlParser::readImportedModules()
{
    while (nextRequiredElementIs(QStringLiteral("ImportedModule")))
        readImportedModule();
}

void AstXmlParser::readImportedModule()
{
    while (m_xmlReader.readNextStartElement())
    {
        if (m_xmlReader.name() == QStringLiteral("ImportedTypes"))
            readImportedTypes();
        else if (m_xmlReader.name() == QStringLiteral("ImportedVariables"))
            readImportedVariables();
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readImportedVariables()
{
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readImportedTypes()
{
    while(nextRequiredElementIs("ImportedType"))
        m_currentDefinitions->addImportedType(m_xmlReader.attributes().value(QStringLiteral("Name")).toString());

    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readExportedVariables()
{
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readExportedTypes()
{
    m_xmlReader.skipCurrentElement();
}

bool AstXmlParser::nextRequiredElementIs(const QString& name)
{
    if (!m_xmlReader.readNextStartElement())
        return false;
    if (m_xmlReader.name() == name)
        return true;
    m_xmlReader.raiseError(QString("XML does not contain expected <%1> element").arg(name));
    return false;
}

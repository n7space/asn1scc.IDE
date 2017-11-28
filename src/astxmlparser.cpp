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

#include <QMap>

#include "data/types/userdefinedtype.h"
#include "data/types/builtintypes.h"

using namespace Asn1Acn::Internal;

AstXmlParser::AstXmlParser(QXmlStreamReader &xmlReader, const SourceMapper &mapper)
    : m_xmlReader(xmlReader)
    , m_currentDefinitions(nullptr)
    , m_pathMapper(mapper)
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
    const auto fileName = m_xmlReader.attributes().value(QStringLiteral("FileName")).toString();
    m_currentFile = m_pathMapper.findByFileName(fileName);
    m_data.insert(std::make_pair(m_currentFile, std::make_unique<Data::File>(m_currentFile)));
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
    m_currentModule = readIdAttribute();
    auto module = std::make_unique<Data::Definitions>(m_currentModule, location);
    m_currentDefinitions = module.get();
    m_data[m_currentFile]->add(std::move(module));
}

void AstXmlParser::readAsn1Module()
{
    createNewAsn1Module();
    readAsn1ModuleChildren();
}

QString AstXmlParser::readIdAttribute()
{
    return m_xmlReader.attributes().value("ID").toString();
}

void AstXmlParser::readVariablesAssignments()
{
    while (nextRequiredElementIs(QStringLiteral("VariableAssignment")))
        readVariableAssignment();
}

void AstXmlParser::readTypeAssignments()
{
    while (nextRequiredElementIs(QStringLiteral("TypeAssignment")))
        readTypeAssignment();
}

void AstXmlParser::readTypeAssignment()
{
    Q_ASSERT(m_currentDefinitions != nullptr);
    const auto location = readLocationFromAttributes();
    const auto name = readNameAttribute();
    auto type = readType();

    m_currentDefinitions->addType(std::make_unique<Data::TypeAssignment>(name, location, std::move(type)));
}

void AstXmlParser::readVariableAssignment()
{
    Q_ASSERT(m_currentDefinitions != nullptr);
    const auto location = readLocationFromAttributes();
    const auto name = readNameAttribute();
    auto type = readType();

    m_currentDefinitions->addVariable(std::make_unique<Data::VariableAssignment>(name, location, std::move(type)));
}

QString AstXmlParser::readReferencedTypeNameAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("ReferencedTypeName")).toString();
}

QString AstXmlParser::readReferencedModuleAttribute()
{
    const auto moduleName = m_xmlReader.attributes().value(QStringLiteral("ReferencedModName")).toString();
    if (moduleName.isEmpty())
        return m_currentModule;
    return moduleName;
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
    const auto moduleName = readIdAttribute();
    while (m_xmlReader.readNextStartElement())
    {
        if (m_xmlReader.name() == QStringLiteral("ImportedTypes"))
            readImportedTypes(moduleName);
        else if (m_xmlReader.name() == QStringLiteral("ImportedVariables"))
            readImportedVariables(moduleName);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readImportedVariables(const QString &moduleName)
{
    while (nextRequiredElementIs(QStringLiteral("ImportedVariable")))
        readImportedVariable(moduleName);
}

void AstXmlParser::readImportedVariable(const QString &moduleName)
{
    m_currentDefinitions->addImportedVariable({moduleName, readNameAttribute()});
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readImportedTypes(const QString &moduleName)
{
    while (nextRequiredElementIs(QStringLiteral("ImportedType")))
        readImportedType(moduleName);
}

void AstXmlParser::readImportedType(const QString &moduleName)
{
    m_currentDefinitions->addImportedType({moduleName, readNameAttribute()});
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

bool AstXmlParser::nextRequiredElementIs(const QString &name)
{
    if (!m_xmlReader.readNextStartElement())
        return false;
    if (m_xmlReader.name() == name)
        return true;
    m_xmlReader.raiseError(QString("XML does not contain expected <%1> element").arg(name));
    return false;
}

Data::SourceLocation AstXmlParser::readLocationFromAttributes()
{
    return { m_currentFile, readLineAttribute(), readCharPossitionInLineAttribute() };
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readType()
{
    if (!nextRequiredElementIs("Type"))
        return {};

    const auto location = readLocationFromAttributes();

    m_xmlReader.readNextStartElement();

    std::unique_ptr<Data::Types::Type> type;

    const auto name = m_xmlReader.name();
    if (name == QStringLiteral("ReferenceType")) {
        type = readUserDefinedTypeReference(location);
        m_xmlReader.skipCurrentElement();
    } else {
        type = Data::Types::BuiltinType::createBuiltinType(name.toString());

        if (name == QStringLiteral("SequenceType"))
            readSequence();
        else if (name == QStringLiteral("SequenceOfType"))
            readSequenceOf();
        else if (name == QStringLiteral("ChoiceType"))
            readChoice();
        else
            m_xmlReader.skipCurrentElement();
    }

    m_xmlReader.skipCurrentElement();
    m_xmlReader.skipCurrentElement();

    return type;
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readUserDefinedTypeReference(const Data::SourceLocation &location)
{
    const QString refName = readReferencedTypeNameAttribute();
    const QString module = readReferencedModuleAttribute();

    auto ref = std::make_unique<Data::TypeReference>(refName, module, location);

    m_data[m_currentFile]->addTypeReference(std::move(ref));

    return std::make_unique<Data::Types::UserdefinedType>(refName, module);
}

void AstXmlParser::readSequence()
{
    while(nextRequiredElementIs(QStringLiteral("SequenceOrSetChild")))
        readType();
}

void AstXmlParser::readSequenceOf()
{
    readType();
}

void AstXmlParser::readChoice()
{
    while(nextRequiredElementIs(QStringLiteral("ChoiceChild")))
        readType();
}

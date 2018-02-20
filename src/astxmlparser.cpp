/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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
    if (nextRequiredElementIs(QStringLiteral("AstRoot")))
        readAstRoot();
    return !m_xmlReader.hasError();
}

void AstXmlParser::readAstRoot()
{
    while (skipToChildElement(QStringLiteral("Asn1File")))
        readAsn1File();
}

void AstXmlParser::readAsn1File()
{
    updateCurrentFile();
    if (nextRequiredElementIs(QStringLiteral("Modules")))
        readModules();
    m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readModules()
{
    while (nextRequiredElementIs(QStringLiteral("Module")))
        readModule();
}

void AstXmlParser::updateCurrentFile()
{
    const auto fileName = m_xmlReader.attributes().value(QStringLiteral("FileName")).toString();
    m_currentFile = m_pathMapper.findByFileName(fileName);
    m_data.insert(std::make_pair(m_currentFile, std::make_unique<Data::File>(m_currentFile)));
}

void AstXmlParser::readModuleChildren()
{
    while (m_xmlReader.readNextStartElement())
    {
        if (m_xmlReader.name() == QStringLiteral("ExportedTypes"))
            readExportedTypes();
        else if (m_xmlReader.name() == QStringLiteral("ExportedValues"))
            readExportedValues();
        else if (m_xmlReader.name() == QStringLiteral("ImportedModules"))
            readImportedModules();
        else if (m_xmlReader.name() == QStringLiteral("TypeAssignments"))
            readTypeAssignments();
        else if (m_xmlReader.name() == QStringLiteral("ValueAssignments"))
            readValueAssignments();
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::createNewModule()
{
    const auto location = readLocationFromAttributes();
    m_currentModule = readNameAttribute();
    auto module = std::make_unique<Data::Definitions>(m_currentModule, location);
    m_currentDefinitions = module.get();
    m_data[m_currentFile]->add(std::move(module));
}

void AstXmlParser::readModule()
{
    createNewModule();
    readModuleChildren();
}

QString AstXmlParser::readIdAttribute()
{
    return m_xmlReader.attributes().value("ID").toString();
}

void AstXmlParser::readValueAssignments()
{
    while (nextRequiredElementIs(QStringLiteral("ValueAssignment")))
        readValueAssignment();
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
    m_xmlReader.skipCurrentElement();

    m_currentDefinitions->addType(std::make_unique<Data::TypeAssignment>(name, location, std::move(type)));
    m_data[m_currentFile]->addTypeReference(std::make_unique<Data::TypeReference>(name, m_currentDefinitions->name(), location));
}

void AstXmlParser::readValueAssignment()
{
    Q_ASSERT(m_currentDefinitions != nullptr);
    const auto location = readLocationFromAttributes();
    const auto name = readNameAttribute();
    auto type = readType();
    m_xmlReader.skipCurrentElement();

    m_currentDefinitions->addValue(std::make_unique<Data::ValueAssignment>(name, location, std::move(type)));
}

QString AstXmlParser::readTypeAssignmentAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("TypeAssignment")).toString();
}

QString AstXmlParser::readModuleAttribute()
{
    return m_xmlReader.attributes().value(QStringLiteral("Module")).toString();
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
        else if (m_xmlReader.name() == QStringLiteral("ImportedValues"))
            readImportedValues(moduleName);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void AstXmlParser::readImportedValues(const QString &moduleName)
{
    while (nextRequiredElementIs(QStringLiteral("ImportedValue")))
        readImportedValue(moduleName);
}

void AstXmlParser::readImportedValue(const QString &moduleName)
{
    m_currentDefinitions->addImportedValue({moduleName, readNameAttribute()});
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

void AstXmlParser::readExportedValues()
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
    if (!skipToChildElement(QStringLiteral("Asn1Type")))
        return {};

    const auto location = readLocationFromAttributes();
    auto type = readTypeDetails(location);

    m_xmlReader.skipCurrentElement();

    return type;
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readTypeDetails(const Data::SourceLocation &location)
{
    if (!m_xmlReader.readNextStartElement())
        return {};

    const auto name = m_xmlReader.name();
    auto type = buildTypeFromName(location, name);

    if (name == QStringLiteral("SEQUENCE"))
        readSequence();
    else if (name == QStringLiteral("SEQUENCE_OF"))
        readSequenceOf();
    else if (name == QStringLiteral("CHOICE"))
        readChoice();
    else
        m_xmlReader.skipCurrentElement();

    return type;
}

std::unique_ptr<Data::Types::Type> AstXmlParser::buildTypeFromName(const Data::SourceLocation &location,
                                                                   const QStringRef &name)
{
    if (name == QStringLiteral("REFERENCE_TYPE"))
        return readReferenceType(location);
    return Data::Types::BuiltinType::createBuiltinType(name.toString());
}

std::unique_ptr<Data::Types::Type> AstXmlParser::readReferenceType(const Data::SourceLocation &location)
{
    const QString refName = readTypeAssignmentAttribute();
    const QString module = readModuleAttribute();

    auto ref = std::make_unique<Data::TypeReference>(refName, module, location);

    m_data[m_currentFile]->addTypeReference(std::move(ref));

    return std::make_unique<Data::Types::UserdefinedType>(refName, module);
}

void AstXmlParser::readSequence()
{
    while (skipToChildElement(QStringLiteral("SEQUENCE_COMPONENT"))) {
        readType();
        m_xmlReader.skipCurrentElement();
    }
}

bool AstXmlParser::skipToChildElement(const QString &name)
{
    while (m_xmlReader.readNextStartElement())
    {
        if (m_xmlReader.name() == name)
            return true;
        else
            m_xmlReader.skipCurrentElement();
    }

    return false;
}

void AstXmlParser::readSequenceOf()
{
    if (readType())
        m_xmlReader.skipCurrentElement();
}

void AstXmlParser::readChoice()
{
    while (skipToChildElement(QStringLiteral("CHOICE_ALTERNATIVE"))) {
        readType();
        m_xmlReader.skipCurrentElement();
    }
}

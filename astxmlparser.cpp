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
    m_currentFile = m_xmlReader.attributes().value(QStringLiteral("FileName")).toString();
    const auto &source = m_pathMapper.findByFileName(m_currentFile);
    m_currentFile = source.filePath();
    m_data.insert(std::make_pair(m_currentFile, std::make_unique<Data::File>(source)));
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
    auto module = std::make_unique<Data::Definitions>(m_currentModule, location);
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
    const auto location = readLocationFromAttributes();
    const auto name = readNameAttribute();
    auto type = readType();

    m_currentDefinitions->add(std::make_unique<Data::TypeAssignment>(name, location, type));
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
    while (nextRequiredElementIs("ImportedType"))
        readImportedType();
}

void AstXmlParser::readImportedType()
{
    m_currentDefinitions->addImportedType(readNameAttribute());
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

namespace
{
Data::Type::Kind mapNameToDataType(const QStringRef &name)
{
    static const QMap<QString, Data::Type::Kind> mapping = {
        { "BooleanType",       Data::Type::Kind::Boolean },
        { "NullType",          Data::Type::Kind::Null },
        { "IntegerType",       Data::Type::Kind::Integer },
        { "RealType",          Data::Type::Kind::Real },
        { "BitStringType",     Data::Type::Kind::BitString },
        { "OctetStringType",   Data::Type::Kind::OctetString },
        { "IA5StringType",     Data::Type::Kind::IA5String },
        { "NumericStringType", Data::Type::Kind::NumericString },
        { "EnumeratedType",    Data::Type::Kind::Enumerated },
        { "ChoiceType",        Data::Type::Kind::Choice },
        { "SequenceType",      Data::Type::Kind::Sequence },
        { "SequenceOfType",    Data::Type::Kind::SequenceOf },
    };
    const auto it = mapping.find(name.toString());
    if (it == mapping.end())
        return Data::Type::Kind::UserDefined;
    return it.value();
}
} // namespace

Data::Type AstXmlParser::readType()
{
    if (!nextRequiredElementIs("Type"))
        return {};

    const auto location = readLocationFromAttributes();

    m_xmlReader.readNextStartElement();
    Data::Type::Kind kind = mapNameToDataType(m_xmlReader.name());

    Data::Type type(kind);
    switch (kind) {
    case Data::Type::Kind::UserDefined:
        readUserDefinedTypeReference(location, type);
        m_xmlReader.skipCurrentElement();
        break;
    case Data::Type::Kind::Sequence:
        readSequence();
        break;
    case Data::Type::Kind::SequenceOf:
        readSequenceOf();
        break;
    default:
        m_xmlReader.skipCurrentElement();
    }

    m_xmlReader.skipCurrentElement();
    m_xmlReader.skipCurrentElement();

    return type;
}

void AstXmlParser::readUserDefinedTypeReference(const Data::SourceLocation &location, Data::Type &type)
{
    const QString refName = readReferencedTypeNameAttribute();
    const QString module = readReferencedModuleAttribute();

    auto ref = std::make_unique<Data::TypeReference>(refName, module, location);

    m_data[m_currentFile]->addTypeReference(std::move(ref));

    type.m_name = refName;
    type.m_module = module;
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

/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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
    auto module = std::unique_ptr<Data::Definitions>(new Data::Definitions(readAsnModuleId(), location));
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
    const auto type = Data::TypeAssignment(readNameAttribute(), location);
    m_currentDefinitions->add(type);

    m_xmlReader.skipCurrentElement();
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

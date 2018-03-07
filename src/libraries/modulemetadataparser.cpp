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
#include "modulemetadataparser.h"

#include <QJsonArray>
#include <QJsonObject>

#include "metadata/module.h"

using namespace Asn1Acn::Internal::Libraries;

ModuleMetadataParser::ModuleMetadataParser(const QByteArray &data)
    : m_document(QJsonDocument::fromJson(data))
{}

namespace {

template<typename Func>
void foreachItem(const QJsonValue &value, Func f)
{
    for (const auto &item : value.toArray())
        f(item);
}

QString readNotEmptyStringField(const QJsonObject &object, const QString &fieldName)
{
    const auto value = object[fieldName].toString();
    if (value.isEmpty())
        throw ModuleMetadataParser::Error("'" + fieldName.toStdString() + "' missing or empty");
    return value;
}

QString readOptionalStringField(const QJsonObject &object,
                                const QString &fieldName,
                                const QString &defaultValue = QString::null)
{
    const auto value = object[fieldName].toString();
    if (value.isEmpty())
        return defaultValue;
    return value;
}

QString readName(const QJsonObject &object)
{
    return readNotEmptyStringField(object, "name");
}

QString readDescription(const QJsonObject &object)
{
    return readOptionalStringField(object, "description");
}

template<typename T>
std::unique_ptr<T> readNamedType(const QJsonObject &object)
{
    return std::make_unique<T>(readName(object), readDescription(object));
}

} // namespace

std::unique_ptr<Metadata::Element> ModuleMetadataParser::readElement(const QJsonObject &object)
{
    auto element = readNamedType<Metadata::Element>(object);
    foreachItem(object["asn1Files"],
                [&element](const QJsonValue &value) { element->addAsn1File(value.toString()); });
    foreachItem(object["additionalFiles"], [&element](const QJsonValue &value) {
        element->addAdditionalFile(value.toString());
    });
    foreachItem(object["conflicts"], [this, &element](const QJsonValue &value) {
        element->addConflict(readReference(value));
    });
    foreachItem(object["requires"], [this, &element](const QJsonValue &value) {
        element->addRequirement(readReference(value));
    });
    return element;
}

std::unique_ptr<Metadata::Submodule> ModuleMetadataParser::readSubmodule(const QJsonObject &object)
{
    auto submodule = readNamedType<Metadata::Submodule>(object);
    m_currentSubmoduleName = submodule->name();
    foreachItem(object["elements"], [this, &submodule](const QJsonValue &value) {
        submodule->addElement(readElement(value.toObject()));
    });
    return submodule;
}

std::unique_ptr<Metadata::Module> ModuleMetadataParser::readModule(const QJsonObject &object)
{
    auto module = readNamedType<Metadata::Module>(object);
    m_currentModuleName = module->name();
    foreachItem(object["submodules"], [this, &module](const QJsonValue &value) {
        module->addSubmodule(readSubmodule(value.toObject()));
    });
    return module;
}

std::unique_ptr<Metadata::Module> ModuleMetadataParser::parse()
{
    if (m_document.isNull())
        throw Error("JSON malformed");
    return readModule(m_document.object());
}

Metadata::Reference ModuleMetadataParser::readReference(const QJsonValue &value) const
{
    if (value.isString())
        return Metadata::Reference(m_currentModuleName, m_currentSubmoduleName, value.toString());
    const auto reference = value.toObject();
    return Metadata::Reference(readOptionalStringField(reference, "module", m_currentModuleName),
                               readNotEmptyStringField(reference, "submodule"),
                               readNotEmptyStringField(reference, "element"));
}

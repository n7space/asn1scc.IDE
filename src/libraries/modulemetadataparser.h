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
#pragma once

#include <stdexcept>
#include <string>
#include <memory>

#include <QJsonDocument>
#include <QByteArray>

#include "metadata/reference.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {

namespace Metadata {
class Module;
class Submodule;
class Element;
}

class ModuleMetadataParser
{
public:
    struct Error : std::runtime_error
    {
        Error(const std::string &msg)
            : std::runtime_error(msg)
        {}
    };

    ModuleMetadataParser(const QByteArray &data);

    std::unique_ptr<Metadata::Module> parse();

private:
    std::unique_ptr<Metadata::Module> readModule(const QJsonObject &object);
    std::unique_ptr<Metadata::Submodule> readSubmodule(const QJsonObject &object);
    std::unique_ptr<Metadata::Element> readElement(const QJsonObject &object);
    Metadata::Reference readReference(const QJsonValue &value) const;

    QJsonDocument m_document;

    QString m_currentModuleName;
    QString m_currentSubmoduleName;
};

} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

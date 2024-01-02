/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include <QString>

#include "sourcelocation.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class TypeReference
{
public:
    explicit TypeReference(const SourceLocation &location = {});
    TypeReference(const QString &name, const QString &module, const SourceLocation &location);

    const SourceLocation &location() const { return m_location; }
    const QString &name() const { return m_name; }
    const QString &module() const { return m_module; }

private:
    SourceLocation m_location;
    QString m_name;
    QString m_module;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

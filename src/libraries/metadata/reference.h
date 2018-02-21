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

#include <QString>

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Metadata {

class Reference
{
public:
    Reference(const QString &module, const QString &submodule, const QString &element)
        : m_module(module)
        , m_submodule(submodule)
        , m_element(element)
    {}

    const QString &module() const { return m_module; }
    const QString &submodule() const { return m_submodule; }
    const QString &element() const { return m_element; }

    bool operator==(const Reference &other) const
    {
        return m_module == other.m_module && m_submodule == other.m_submodule
               && m_element == other.m_element;
    }

private:
    QString m_module;
    QString m_submodule;
    QString m_element;
};

} // namespace Metadata
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

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
#pragma once

#include <QList>

#include "submodule.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Metadata {

class Module
{
public:
    Module(const QString &name, const QString &description)
        : m_name(name)
        , m_description(description)
    {}

    const QString &name() const { return m_name; }
    const QString &description() const { return m_description; }
    const QList<Submodule> &submodules() const { return m_submodules; }

    void addSubmodule(const Submodule &submodule) { m_submodules.append(submodule); }

private:
    QString m_name;
    QString m_description;
    QList<Submodule> m_submodules;
};

} // namespace Metadata
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

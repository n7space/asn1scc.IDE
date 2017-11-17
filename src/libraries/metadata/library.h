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

#include "module.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Metadata {

class Library
{
public:
    Library(const QString &name)
        : m_name(name)
    {}

    const QString &name() const { return m_name; }
    const QList<Module> &modules() const { return m_modules; }

    void addModule(const Module &module) { m_modules.append(module); }

private:
    QString m_name;
    QList<Module> m_modules;
};

} // namespace Metadata
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

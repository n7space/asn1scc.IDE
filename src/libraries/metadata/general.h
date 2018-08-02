/****************************************************************************
**
** Copyright (C) 2017-2018 N7 Space sp. z o. o.
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

class General
{
public:
    General() = default;
    General(const QString &name, const QString &path)
        : m_name(name)
        , m_path(path)
    {}

    const QString &name() const { return m_name; }
    const QString &path() const { return m_path; }

    const QString &version() const { return m_version; }
    void setVersion(const QString &version) { m_version = version; }

    const QString &description() const { return m_description; }
    void setDescription(const QString &desc) { m_description = desc; }

    const QString &license() const { return m_license; }
    void setLicense(const QString &lic) { m_license = lic; }

    const QString &vendor() const { return m_vendor; }
    void setVendor(const QString &vendor) { m_vendor = vendor; }

private:
    QString m_name;
    QString m_path;
    QString m_version;
    QString m_description;
    QString m_license;
    QString m_vendor;
};

} // namespace Metadata
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

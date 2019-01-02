/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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

#include <memory>

#include <QString>
#include <QStringList>

#include "settings.h"

namespace Asn1Acn {
namespace Internal {
namespace Settings {

class Libraries : public Settings
{
    Q_OBJECT
public:
    Libraries();
    virtual ~Libraries();

    QString name() const override;

    QStringList libPaths() const { return detectedLibPaths() + manualLibPaths(); }

    const QStringList &detectedLibPaths() const { return m_detectedLibPaths; }

    const QStringList &manualLibPaths() const { return m_manualLibPaths; }
    void setManualLibPaths(const QStringList &list) { m_manualLibPaths = list; }

private:
    void saveOptionsTo(QSettings *s) const override;
    void loadOptionsFrom(QSettings *s) override;

    QStringList m_manualLibPaths;
    QStringList m_detectedLibPaths;
};

using LibrariesPtr = std::shared_ptr<Libraries>;
using LibrariesConstPtr = std::shared_ptr<const Libraries>;

} // namespace Settings
} // namespace Internal
} // namespace Asn1Acn

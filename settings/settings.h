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

#pragma once

#include <memory>

#include <QSettings>
#include <QString>
#include <QObject>

#include <coreplugin/icore.h>

namespace Asn1Acn {
namespace Internal {
namespace Settings {

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings() = default;
    virtual ~Settings();

    virtual QString name() const = 0;

    void saveTo(QSettings *s);
    void loadFrom(QSettings *s);

signals:
    void changed();

protected:
    virtual void saveOptionsTo(QSettings *s) = 0;
    virtual void loadOptionsFrom(QSettings *s) = 0;
};

template <typename Type>
std::shared_ptr<Type> load()
{
    auto r = std::make_shared<Type>();
    r->loadFrom(Core::ICore::settings());
    return r;
}

template <typename Type>
void save(std::shared_ptr<Type> settings)
{
    settings->saveTo(Core::ICore::settings());
}

} // namespace Settings
} // namespace Internal
} // namespace Asn1Acn

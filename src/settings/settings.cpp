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

#include "settings.h"

#include <asn1acnconstants.h>

using namespace Asn1Acn::Internal::Settings;

Settings::~Settings() {}

void Settings::saveTo(QSettings *s) const
{
    s->beginGroup(Constants::SETTINGS_GROUP);
    s->beginGroup(name());
    saveOptionsTo(s);
    s->endGroup();
    s->endGroup();
    s->sync();
}

void Settings::loadFrom(QSettings *s)
{
    s->beginGroup(Constants::SETTINGS_GROUP);
    s->beginGroup(name());
    loadOptionsFrom(s);
    s->endGroup();
    s->endGroup();
}

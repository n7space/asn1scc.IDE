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

#include "service.h"

using namespace Asn1Acn::Internal::Settings;

static const char PATH[] = "Path";
static const char BASE_URI[] = "BaseUri";
static const char STAY_ALIVE_PERIOD[] = "StayAlivePeriod";

Service::~Service()
{
}

QString Service::name() const
{
    return QLatin1String("Service");
}

void Service::saveOptionsTo(QSettings *s)
{
    s->setValue(PATH, path);
    s->setValue(BASE_URI, baseUri); // TODO must end with /
    s->setValue(STAY_ALIVE_PERIOD, stayAlivePeriod);
}

void Service::loadOptionsFrom(QSettings *s)
{
    path = s->value(PATH, "/opt/asn1sccDaemon/asn1scc/Daemon/bin/Debug/Daemon.exe").toString(); // TODO good default, TODO windows support
    baseUri = s->value(BASE_URI, "http::/localhost:9749/").toString(); // TODO windows default!
    stayAlivePeriod = s->value(STAY_ALIVE_PERIOD, 1000).toInt();
}

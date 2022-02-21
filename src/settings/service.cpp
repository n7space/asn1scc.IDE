/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
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

#include "service.h"

#include <QDir>

#include <plugins/coreplugin/icore.h>
#include <utils/hostosinfo.h>

using namespace Asn1Acn::Internal::Settings;

static const char PATH[] = "Path";
static const char LISTENING_URI[] = "ListeningUri";
static const char STAY_ALIVE_PERIOD[] = "StayAlivePeriod";

Service::~Service() {}

QString Service::name() const
{
    return QLatin1String("Service");
}

void Service::saveOptionsTo(QSettings *s) const
{
    s->setValue(PATH, QDir::fromNativeSeparators(path()));
    s->setValue(LISTENING_URI, listeningUri());
    s->setValue(STAY_ALIVE_PERIOD, stayAlivePeriod());
}

static QString defaultDaemonPath()
{
    return Core::ICore::libexecPath() + QLatin1String("/asn1scc/daemon/asn1.daemon.exe");
}

void Service::loadOptionsFrom(QSettings *s)
{
    setPath(s->value(PATH, defaultDaemonPath()).toString());
    setListeningUri(s->value(LISTENING_URI,
                             Utils::HostOsInfo::isWindowsHost()
                                 ? "http://+:80/Temporary_Listen_Addresses/asn1scc.IDE/"
                                 : "http://localhost:9749/")
                        .toString());
    setStayAlivePeriod(s->value(STAY_ALIVE_PERIOD, 1000).toInt());
}

QString Service::serviceUri() const
{
    auto uri = listeningUri();
    if (Utils::HostOsInfo::isWindowsHost())
        return uri.replace("//+", "//localhost");
    return uri;
}

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
#include "maltester.h"

#include <QDir>

#include <plugins/coreplugin/icore.h>

using namespace Asn1Acn::Internal::Settings;

static const char PATH[] = "MalTesterPath";

MalTester::~MalTester() {}

QString MalTester::name() const
{
    return QLatin1String("MalTester");
}

void MalTester::saveOptionsTo(QSettings *s) const
{
    s->setValue(PATH, QDir::fromNativeSeparators(path()));
}

static QString defaultMalTesterPath()
{
    return Core::ICore::libexecPath() + QLatin1String("/MalTester/");
}

void MalTester::loadOptionsFrom(QSettings *s)
{
    setPath(s->value(PATH, defaultMalTesterPath()).toString());
}

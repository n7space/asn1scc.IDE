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
#include "libraries.h"

#include <QDir>

#include <coreplugin/icore.h>

using namespace Asn1Acn::Internal::Settings;

static const char LIBPATH_PATH[] = { "Paths" };
static const char RELATIVE_ASN1LIB_PATH[] = "/asn1acn/libs";

namespace {

QString globalLibRootDir()
{
    return Core::ICore::resourcePath() + RELATIVE_ASN1LIB_PATH;
}

QString userLibRootDir()
{
    return Core::ICore::userResourcePath() + RELATIVE_ASN1LIB_PATH;
}

QStringList listLibs(const QString &path)
{
    const auto dir = QDir(path);
    if (!dir.exists())
        return {};
    return dir.entryList(QDir::AllDirs);
}

} // namespace

Libraries::Libraries()
{
    m_detectedLibPaths += listLibs(globalLibRootDir());
    m_detectedLibPaths += listLibs(userLibRootDir());
}

Libraries::~Libraries()
{
}

QString Libraries::name() const
{
    return QLatin1String("Libraries");
}

void Libraries::saveOptionsTo(QSettings *s) const
{
    s->setValue(LIBPATH_PATH, manualLibPaths());
}

void Libraries::loadOptionsFrom(QSettings *s)
{
    setManualLibPaths(s->value(LIBPATH_PATH).toStringList());
}

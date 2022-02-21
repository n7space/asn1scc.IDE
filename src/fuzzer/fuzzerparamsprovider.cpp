/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
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
#include "fuzzerparamsprovider.h"

#include <projectexplorer/target.h>

#include "kitaspect.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Fuzzer;

FuzzerParamsProvider::FuzzerParamsProvider(Settings::FuzzerConstPtr settings)
    : m_settings(settings)
{}

void FuzzerParamsProvider::setRootTypeName(const QString &name)
{
    m_rootTypeName = name;
}

void FuzzerParamsProvider::setOutputPath(const QString &path)
{
    m_outputPath = path;
}

QString FuzzerParamsProvider::asn1sccPath(const ProjectExplorer::Target *target) const
{
    return KitAspect::hasAsn1Exe(target->kit())
               ? KitAspect::asn1Exe(target->kit()).toString()
               : QString();
}

QString FuzzerParamsProvider::fuzzerPath() const
{
    return m_settings->path();
}

QString FuzzerParamsProvider::outputPath() const
{
    return m_outputPath;
}

QString FuzzerParamsProvider::rootTypeName() const
{
    return m_rootTypeName;
}

QString FuzzerParamsProvider::asn1acnFiles() const
{
    return {};
}

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
#pragma once

#include <memory>

#include <QString>

#include "settings/fuzzer.h"

namespace ProjectExplorer {
class Target;
}

namespace Asn1Acn {
namespace Internal {
namespace Fuzzer {

class FuzzerParamsProvider
{
public:
    FuzzerParamsProvider(Settings::FuzzerConstPtr settings);

    QString asn1sccPath(const ProjectExplorer::Target *target) const;
    QString fuzzerPath() const;
    QString outputPath() const;
    QString rootTypeName() const;
    QString asn1acnFiles() const;

    void setRootTypeName(const QString &name);
    void setOutputPath(const QString &path);

private:
    QString m_outputPath;
    QString m_rootTypeName;

    Settings::FuzzerConstPtr m_settings;
};

using FuzzerParamsProviderPtr = std::shared_ptr<FuzzerParamsProvider>;

} // namespace Fuzzer
} // namespace Internal
} // namespace Asn1Acn

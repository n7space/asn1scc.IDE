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

#include <map>
#include <memory>
#include <vector>

#include "file.h"
#include "node.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class Project : public Node
{
public:
    Project(const QString &projectName);
    ~Project() override;

    void accept(Visitor &visitor) const override;

    void add(std::unique_ptr<File> file);
    void remove(const QString &path);

    using Files = std::vector<std::unique_ptr<File>>;
    const Files &files() const { return m_files; }
    File *file(const QString &path) const;

    int buildersCount() const { return m_buildersCount; }
    void setBuildersCount(const int buildersCount) { m_buildersCount = buildersCount; }

private:
    int m_buildersCount;

    Files m_files;
    std::map<QString, File *> m_filesByPathMap;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

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
#pragma once

#include <memory>
#include <map>
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

    const QString &name() const { return m_name; }

    void add(std::unique_ptr<File> file);

    using Files = std::vector<std::unique_ptr<File>>;
    const Files &files() const { return m_files; }
    const File* file(const QString &path) const;

private:
    QString m_name;
    Files m_files;
    std::map<QString, File*> m_filesByPathMap;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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

#include <QString>

#include "node.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class Project;

class Root : public Node
{
public:
    Root();
    ~Root() override;

    void accept(Visitor &visitor) const override;

    void add(std::unique_ptr<Project> project);
    void remove(const QString &name);

    using Projects = std::vector<std::unique_ptr<Project>>;
    const Projects &projects() const { return m_projects; }

    Project *project(const QString &name) const;

private:
    Projects m_projects;
    std::map<QString, Project *> m_nameToProjectMap;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

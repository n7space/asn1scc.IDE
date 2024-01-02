/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
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
#include "root.h"

#include <QFileInfo>

#include "project.h"
#include "visitor.h"

using namespace Asn1Acn::Internal::Data;

Root::Root()
    : Node("ROOT", {})
{}

Root::~Root() {}

void Root::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

void Root::add(std::unique_ptr<Project> project)
{
    project->setParent(this);
    m_nameToProjectMap[project->name()] = project.get();
    m_projects.push_back(std::move(project));
}

void Root::remove(const QString &name)
{
    const auto mapIt = m_nameToProjectMap.find(name);
    if (mapIt != m_nameToProjectMap.end())
        m_nameToProjectMap.erase(mapIt);

    for (auto vecIt = m_projects.begin(); vecIt != m_projects.end(); vecIt++) {
        if ((*vecIt)->name() == name) {
            m_projects.erase(vecIt);
            break;
        }
    }
}

Project *Root::project(const QString &name) const
{
    auto it = m_nameToProjectMap.find(name);
    return it != m_nameToProjectMap.end() ? it->second : nullptr;
}

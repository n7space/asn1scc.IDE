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

#include <memory>
#include <vector>

#include "submodule.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Metadata {

class Module : public LibraryNode
{
public:
    Module(const QString &name, const QString &description)
        : LibraryNode(name, description)
    {}

    using Submodules = std::vector<std::unique_ptr<Submodule>>;

    const Submodules &submodules() const { return m_submodules; }

    void addSubmodule(std::unique_ptr<Submodule> submodule)
    {
        submodule->setParent(this);
        m_submodules.push_back(std::move(submodule));
    }

    LibraryNode *child(int num) const override
    {
        return num < childrenCount() ? m_submodules[num].get() : nullptr;
    }

    int childrenCount() const override { return static_cast<int>(m_submodules.size()); }

    int childIndex(const LibraryNode *child) const override
    {
        return findChildIndex<Submodules>(submodules(), child);
    }

private:
    Submodules m_submodules;
};

} // namespace Metadata
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

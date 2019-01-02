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

#include "element.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {
namespace Metadata {

class Submodule : public LibraryNode
{
public:
    Submodule(const QString &name, const QString &description)
        : LibraryNode(name, description)
    {}

    using Elements = std::vector<std::unique_ptr<Element>>;

    const Elements &elements() const { return m_elements; }

    void addElement(std::unique_ptr<Element> element)
    {
        element->setParent(this);
        m_elements.push_back(std::move(element));
    }

    LibraryNode *child(int num) const override
    {
        return num < childrenCount() ? m_elements[num].get() : nullptr;
    }

    int childrenCount() const override { return static_cast<int>(m_elements.size()); }

    int childIndex(const LibraryNode *child) const override
    {
        return LibraryNode::findChildIndex<Elements>(elements(), child);
    }

private:
    Elements m_elements;
};

} // namespace Metadata
} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

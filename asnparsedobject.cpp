/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

#include "asnparsedobject.h"

#include "utils/qtcassert.h"

using namespace Asn1Acn::Internal;

AsnParsedObject::AsnParsedObject(const QVariant& data) :
    m_data(data),
    m_parent(nullptr)
{
}

AsnParsedObject::~AsnParsedObject()
{
    detachChildren();
}

int AsnParsedObject::childrenCount() const
{
    return m_children.count();
}

AsnParsedObject* AsnParsedObject::childAt(int idx) const
{
    return idx < m_children.size() ? m_children[idx].get() : nullptr;
}

void AsnParsedObject::addChild(std::shared_ptr<AsnParsedObject> child)
{
    QTC_ASSERT(child->m_parent == this || child->m_parent == nullptr, return);

    m_children.push_back(child);
    child->m_parent = this;
}

void AsnParsedObject::detachChildren()
{
    foreach (std::shared_ptr<AsnParsedObject> child, m_children)
        child->m_parent = nullptr;

    m_children.clear();
}

const QVariant &AsnParsedObject::data() const
{
    return m_data;
}

const AsnParsedObject* AsnParsedObject::parent() const
{
    return m_parent;
}

int AsnParsedObject::columnCount() const
{
    return 1;
}

int AsnParsedObject::row() const
{
    int idx = 0;
    foreach (const std::shared_ptr<AsnParsedObject> &obj, m_parent->m_children) {
        if (obj.get() == this)
            return idx;

        idx++;
    }

    return 0;
}

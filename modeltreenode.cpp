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

#include "modeltreenode.h"

#include "utils/qtcassert.h"

using namespace Asn1Acn::Internal;

ModelTreeNode::ModelTreeNode(const QString &id, const Data::SourceLocation location) :
    m_id(id),
    m_parent(nullptr),
    m_location(location)
{
}

int ModelTreeNode::childrenCount() const
{
    return m_children.count();
}

ModelTreeNode::ModelTreeNodePtr ModelTreeNode::childAt(int idx) const
{
    return idx < m_children.size() ? m_children[idx] : nullptr;
}

ModelTreeNode::ModelTreeNodePtr ModelTreeNode::getChildByName(const QString &name) const
{
    QList<ModelTreeNode::ModelTreeNodePtr>::const_iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it) {
        if ((*it)->m_id == name)
            return *it;
    }

    return nullptr;
}

void ModelTreeNode::addChild(ModelTreeNode::ModelTreeNodePtr child)
{
    QTC_ASSERT(child->m_parent == this || child->m_parent == nullptr, return );

    m_children.push_back(child);
    child->m_parent = this;
}

void ModelTreeNode::removeChildByName(const QString &name)
{
    QList<ModelTreeNode::ModelTreeNodePtr>::iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it) {
        if ((*it)->m_id == name) {
            m_children.erase(it);
            return;
        }
    }
}

void ModelTreeNode::removeChildren()
{
    m_children.clear();
}

QVariant ModelTreeNode::data() const
{
    return QVariant(m_id);
}

QString ModelTreeNode::id() const
{
    return m_id;
}

const ModelTreeNode *ModelTreeNode::parent() const
{
    return m_parent;
}

int ModelTreeNode::columnCount() const
{
    return 1;
}

int ModelTreeNode::row() const
{
    int idx = 0;
    foreach (const ModelTreeNode::ModelTreeNodePtr &obj, m_parent->m_children) {
        if (obj.get() == this)
            return idx;

        idx++;
    }

    return 0;
}

const Data::SourceLocation &ModelTreeNode::getSourceLocation() const
{
    return m_location;
}

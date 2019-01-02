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

#include "visitor.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

template<typename Value>
class VisitorWithValue : public Visitor
{
public:
    using ValueType = Value;

    const Value &value() const { return m_value; }

    void visit(const Definitions &defs) override final { m_value = valueFor(defs); }
    void visit(const File &file) override final { m_value = valueFor(file); }
    void visit(const TypeAssignment &type) override final { m_value = valueFor(type); }
    void visit(const ValueAssignment &value) override final { m_value = valueFor(value); }
    void visit(const Project &project) override final { m_value = valueFor(project); }
    void visit(const Root &root) override final { m_value = valueFor(root); }

protected:
    virtual Value valueFor(const Root &root) const = 0;
    virtual Value valueFor(const Definitions &defs) const = 0;
    virtual Value valueFor(const File &file) const = 0;
    virtual Value valueFor(const TypeAssignment &type) const = 0;
    virtual Value valueFor(const ValueAssignment &value) const = 0;
    virtual Value valueFor(const Project &project) const = 0;

private:
    Value m_value;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

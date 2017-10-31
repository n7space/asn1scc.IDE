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

#include <map>
#include <vector>
#include <memory>

#include <QString>
#include <QStringList>

#include "typeassignment.h"
#include "importedtype.h"
#include "node.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class Definitions : public Node
{
public:
    Definitions(const QString &name, const SourceLocation &location);
    ~Definitions() override;

    void accept(Visitor &visitor) const override;

    const QString &name() const { return m_name; }

    void add(std::unique_ptr<TypeAssignment> type);
    void addImportedType(const ImportedType &type);

    using Types = std::vector<std::unique_ptr<TypeAssignment>>;
    using ImportedTypes = std::vector<ImportedType>;

    const Types &types() const { return m_types; }
    const TypeAssignment *type(const QString &name) const;
    const ImportedTypes &importedTypes() const { return m_importedTypes; }

private:
    QString m_name;
    Types m_types;
    std::map<QString, TypeAssignment*> m_typeByNameMap;
    ImportedTypes m_importedTypes;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

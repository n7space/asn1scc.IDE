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

#include "definitions.h"
#include "node.h"
#include "typereference.h"
#include "errormessage.h"

namespace Asn1Acn {
namespace Internal {
namespace Data {

class File : public Node
{
public:
    File(const QString &path);
    ~File() override;

    void accept(Visitor &visitor) const override;

    void add(std::unique_ptr<Definitions> defs);
    void addTypeReference(std::unique_ptr<TypeReference> ref);
    void addErrorMessage(const ErrorMessage &message);

    using DefinitionsList = std::vector<std::unique_ptr<Definitions>>;
    using ReferencesMap = std::multimap<int, std::unique_ptr<TypeReference>>;
    using ErrorList = std::vector<ErrorMessage>;

    const DefinitionsList &definitionsList() const { return m_definitionsList; }
    const Definitions *definitions(const QString &name) const;

    const ReferencesMap &references() const { return m_typeReferences; }
    const ErrorList &errors() const { return m_errorList; }

    void clearReferences();
    void clearErrors();

private:
    DefinitionsList m_definitionsList;
    ReferencesMap m_typeReferences;
    ErrorList m_errorList;

    std::map<QString, Definitions *> m_definitionsByNameMap;
};

} // namespace Data
} // namespace Internal
} // namespace Asn1Acn

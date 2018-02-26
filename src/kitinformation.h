/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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

#include <projectexplorer/kitmanager.h>

namespace Asn1Acn {
namespace Internal {

class KitInformation : public ProjectExplorer::KitInformation
{
public:
    using Kit = ProjectExplorer::Kit;
    using Task = ProjectExplorer::Task;

    KitInformation();

    static Core::Id id();
    static bool hasAsn1Exe(const Kit *k);
    static Utils::FileName asn1Exe(const Kit *k);
    static void setAsn1Exe(Kit *k, const Utils::FileName &v);

    QVariant defaultValue(const Kit *k) const override;
    QList<Task> validate(const Kit *k) const override;

    ItemList toUserOutput(const Kit *kit) const override;
    ProjectExplorer::KitConfigWidget *createConfigWidget(Kit *kit) const override;

    void addToEnvironment(const Kit *k, Utils::Environment &env) const override;
    void addToMacroExpander(Kit *kit, Utils::MacroExpander *expander) const override;
};

} // namespace Internal
} // namespace Asn1Acn

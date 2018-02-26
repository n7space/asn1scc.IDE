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
#include "kitinformation.h"

#include <QFileInfo>

#include <coreplugin/icore.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/environment.h>
#include <utils/macroexpander.h>

#include "kitconfigwidget.h"

using namespace Asn1Acn::Internal;
using ProjectExplorer::Kit;

KitInformation::KitInformation()
{
    setObjectName(QLatin1Literal("Asn1SccKitInformation"));
    setId(id());
    setPriority(12000);
}

Core::Id KitInformation::id()
{
    return "Asn1Scc.KitInformation.Asn1Exe";
}

bool KitInformation::hasAsn1Exe(const Kit *k)
{
    if (k)
        return !k->value(id()).toString().isEmpty();
    return false;
}

Utils::FileName KitInformation::asn1Exe(const Kit *k)
{
    if (!k)
        return Utils::FileName();
    return Utils::FileName::fromString(k->value(id()).toString());
}

void KitInformation::setAsn1Exe(Kit *k, const Utils::FileName &v)
{
    if (k)
        k->setValue(id(), v.toString());
}

QVariant KitInformation::defaultValue(const Kit *k) const
{
    Q_UNUSED(k);
    const QString path = Core::ICore::libexecPath() + QLatin1String("/asn1scc/asn1.exe");
    return path;
}

QList<KitInformation::Task> KitInformation::validate(const Kit *k) const
{
    QList<Task> result;
    const Utils::FileName asn1 = KitInformation::asn1Exe(k);
    if (asn1.isEmpty())
        return result;

    const QFileInfo fi = asn1.toFileInfo();
    const auto categoryId = Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM);

    if (!fi.exists()) {
        result << Task(Task::Warning,
                       tr("ASN1SCC \"%1\" does not exist in the file system.")
                           .arg(asn1.toUserOutput()),
                       Utils::FileName(),
                       -1,
                       categoryId);
    } else if (!fi.isExecutable()) {
        result << Task(Task::Warning,
                       tr("ASN1SCC \"%1\" is not an executable.").arg(asn1.toUserOutput()),
                       Utils::FileName(),
                       -1,
                       categoryId);
    }
    return result;
}

KitInformation::ItemList KitInformation::toUserOutput(const Kit *kit) const
{
    return ItemList() << qMakePair(tr("ASN1SCC"), asn1Exe(kit).toUserOutput());
}

ProjectExplorer::KitConfigWidget *KitInformation::createConfigWidget(Kit *kit) const
{
    return new KitConfigWidget(kit, this);
}

void KitInformation::addToEnvironment(const Kit *k, Utils::Environment &env) const
{
    env.set(QLatin1String("ASN1SCC"), asn1Exe(k).toUserOutput());
}

void KitInformation::addToMacroExpander(Kit *kit, Utils::MacroExpander *expander) const
{
    expander->registerFileVariables("Asn1Scc", tr("ASN1SCC Path"), [kit]() -> QString {
        return asn1Exe(kit).toString();
    });
}

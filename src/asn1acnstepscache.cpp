/****************************************************************************
**
** Copyright (C) 2018-2024 N7 Space sp. z o. o.
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

#include "asn1acnstepscache.h"

#include <projectexplorer/project.h>
#include <projectexplorer/session.h>

using namespace Asn1Acn::Internal;

Asn1AcnStepsCache::Asn1AcnStepsCache(QObject *parent)
    : QObject(parent)
{
    connect(ProjectExplorer::SessionManager::instance(),
            &ProjectExplorer::SessionManager::aboutToRemoveProject,
            this,
            &Asn1AcnStepsCache::onAboutToRemoveProject);
}

void Asn1AcnStepsCache::add(const QString &id, Asn1AcnBuildStep *step)
{
    m_steps.insert(id, step);
}

Asn1AcnBuildStep *Asn1AcnStepsCache::get(const QString &id) const
{
    return m_steps.contains(id) ? m_steps.value(id) : nullptr;
}

void Asn1AcnStepsCache::onAboutToRemoveProject(ProjectExplorer::Project *p)
{
    m_steps.remove(p->displayName());
}

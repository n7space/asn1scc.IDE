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

#include "vcshandler.h"

#include <stdexcept>

#include <coreplugin/iversioncontrol.h>
#include <coreplugin/vcsmanager.h>

#include <projectexplorer/project.h>
#include <projectexplorer/session.h>

#include <utils/qtcassert.h>

using namespace Asn1Acn::Internal::Libraries;
using namespace Asn1Acn::Internal::Libraries::Wizard;

VcsHandler::VcsHandler(QObject *parent)
    : QObject(parent)
    , m_index(-1)
    , m_createRepo(false)
{
    updateVcsList();

    connect(Core::VcsManager::instance(),
            &Core::VcsManager::configurationChanged,
            this,
            &VcsHandler::onConfigChanged);
}

const QList<Core::IVersionControl *> VcsHandler::vcsList() const
{
    return m_vcs;
}

void VcsHandler::setCurrentIndex(int index)
{
    m_index = index;
}

void VcsHandler::addToVcs(const QStringList &files)
{
    if (m_index < 0)
        return;

    const auto vcs = m_vcs[m_index];

    if (m_createRepo) {
        QTC_ASSERT(vcs->supportsOperation(Core::IVersionControl::CreateRepositoryOperation),
                   return );

        const auto projectPath
            = ProjectExplorer::SessionManager::startupProject()->projectDirectory().toString();
        if (!vcs->vcsCreateRepository(projectPath))
            throw std::runtime_error("Could not create repository in \'" + projectPath.toStdString()
                                     + "\'");
    }

    QTC_ASSERT(vcs->supportsOperation(Core::IVersionControl::AddOperation), return );
    for (const auto &file : files)
        if (!vcs->vcsAdd(file))
            throw std::runtime_error("Failed to add \'" + file.toStdString()
                                     + "\' to version control system");
}

void VcsHandler::onConfigChanged()
{
    updateVcsList();
    emit vcsListChanged();
}

namespace {
Core::IVersionControl *vcsForCurrentProject()
{
    const auto projectDir
        = ProjectExplorer::SessionManager::startupProject()->projectDirectory().toString();
    return Core::VcsManager::findVersionControlForDirectory(projectDir);
}
} // namespace

void VcsHandler::updateVcsList()
{
    m_vcs.clear();

    const auto allVcs = Core::VcsManager::versionControls();
    if (allVcs.isEmpty())
        return;

    const auto currentVcs = vcsForCurrentProject();
    if (currentVcs != nullptr) {
        m_createRepo = false;
        if (currentVcs->supportsOperation(Core::IVersionControl::AddOperation))
            m_vcs.append(currentVcs);
    } else {
        m_createRepo = true;
        for (const auto &vcs : Core::VcsManager::versionControls())
            if (vcs->supportsOperation(Core::IVersionControl::CreateRepositoryOperation))
                m_vcs.append(vcs);
    }
}

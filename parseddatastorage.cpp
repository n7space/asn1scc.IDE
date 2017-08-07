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

#include "parseddatastorage.h"

#include "utils/qtcassert.h"

using namespace Asn1Acn::Internal;

ParsedDataStorage *ParsedDataStorage::instance()
{
    static ParsedDataStorage instance_;
    return &instance_;
}

std::shared_ptr<ParsedDocument>
ParsedDataStorage::getFileForPath(const QString &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);

    return m_documents.contains(filePath) ? m_documents.value(filePath) : nullptr;
}

void ParsedDataStorage::addProject(const QString &projectName)
{
    QMutexLocker locker(&m_documentsMutex);

    Project p;
    m_projects.insert(projectName, p);
}

void ParsedDataStorage::removeProject(const QString &projectName)
{
    QMutexLocker locker(&m_documentsMutex);

    QList<std::shared_ptr<ParsedDocument>> files = getFilesFromProjectInternal(projectName);
    foreach (const std::shared_ptr<ParsedDocument> &file, files)
        removeFileFromProjectInternal(projectName, file->source().getPath());

    m_projects.remove(projectName);
}

QStringList ParsedDataStorage::getProjectsForFile(const QString &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);
    return getProjectsForFileInternal(filePath);
}

QList<std::shared_ptr<ParsedDocument>> ParsedDataStorage::getFilesFromProject(const QString &projectName) const
{
    QMutexLocker locker(&m_documentsMutex);
    return getFilesFromProjectInternal(projectName);
}

void ParsedDataStorage::addFileToProject(const QString &projectName, std::unique_ptr<ParsedDocument> file)
{
    std::shared_ptr<ParsedDocument> newFile = std::move(file);
    QString filePath = newFile->source().getPath();

    {
        QMutexLocker locker(&m_documentsMutex);

        refreshFileInProjects(newFile, filePath);

        Project &p = m_projects[projectName];
        if (!p.contains(filePath))
            p.insert(filePath, newFile);

        m_documents.insert(filePath, newFile);
    }

    emit fileUpdated(filePath, newFile);
}

void ParsedDataStorage::removeFileFromProject(const QString &projectName, const QString &filePath)
{
    QMutexLocker locker(&m_documentsMutex);
    removeFileFromProjectInternal(projectName, filePath);
}

void ParsedDataStorage::refreshFileInProjects(std::shared_ptr<ParsedDocument> file, const QString &filePath)
{
    QStringList projects = getProjectsForFileInternal(filePath);
    foreach (const QString &project, projects)
        m_projects[project].insert(filePath, file);
}

void ParsedDataStorage::removeFileFromProjectInternal(const QString &projectName, const QString &filePath)
{
    Project &p = m_projects[projectName];
    if (p.contains(filePath))
        p.remove(filePath);

    QStringList projects = getProjectsForFileInternal(filePath);
    if (projects.empty())
        m_documents.remove(filePath);
}

QStringList ParsedDataStorage::getProjectsForFileInternal(const QString &filePath) const
{
    QStringList res;

    QHash<QString, Project>::const_iterator it = m_projects.begin();
    while (it != m_projects.end()) {
        if (it.value().contains(filePath))
            res.append(it.key());

        it++;
    }

    return res;
}

QList<std::shared_ptr<ParsedDocument>> ParsedDataStorage::getFilesFromProjectInternal(const QString &projectName) const
{
    if (!m_projects.contains(projectName))
        return QList<std::shared_ptr<ParsedDocument>>();

    Project p = m_projects.value(projectName);

    return p.values();
}

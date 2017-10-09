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

#include "parseddatastorage.h"

#include "utils/qtcassert.h"

#include "data/project.h"
#include "data/file.h"

using namespace Asn1Acn::Internal;

ParsedDataStorage::ParsedDataStorage()
    : m_root(std::make_unique<Data::Root>())
{
}

ParsedDataStorage *ParsedDataStorage::instance()
{
    static ParsedDataStorage instance_;
    return &instance_;
}

const Data::File *ParsedDataStorage::getFileForPath(const QString &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getFileForPathInternal(filePath);
}

void ParsedDataStorage::addProject(const QString &projectName)
{
    QMutexLocker locker(&m_documentsMutex);

    m_root->add(std::make_unique<Data::Project>(projectName));
}

void ParsedDataStorage::removeProject(const QString &projectName)
{
    QMutexLocker locker(&m_documentsMutex);

    m_root->remove(projectName);
}

QStringList ParsedDataStorage::getProjectsForFile(const QString &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getProjectsForFileInternal(filePath);
}

const std::vector<std::shared_ptr<Data::File>> ParsedDataStorage::getFilesFromProject(const QString &projectName) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getFilesFromProjectInternal(projectName);
}

Data::TypeReference ParsedDataStorage::getTypeReference(const QString &path, const int line, const int col) const
{
    QMutexLocker locker(&m_documentsMutex);

    return m_typeReferences.getTypeReference(path, line, col);
}

Data::SourceLocation ParsedDataStorage::getDefinitionLocation(const QString &path, const QString &typeAssignmentName, const QString &definitionsName) const
{
    QMutexLocker locker(&m_documentsMutex);

    const auto file = getFileForPathInternal(path);
    if (file == nullptr)
        return Data::SourceLocation();

    const auto &definitions = file->definitions(definitionsName);
    if (!definitions)
        return {};

    return getLocationFromModule(*definitions, typeAssignmentName);
}

void ParsedDataStorage::addFileToProject(const QString &projectName, std::unique_ptr<Data::File> file)
{
    QMutexLocker locker(&m_documentsMutex);

    auto project = m_root->project(projectName);
    if (project == nullptr)
        return;

    std::shared_ptr<Data::File> newFile = std::move(file);
    QString filePath = newFile->source().filePath();

    project->add(newFile);
    m_typeReferences.addReference(filePath, *newFile);

    emit fileUpdated(filePath, newFile);
}

void ParsedDataStorage::removeFileFromProject(const QString &projectName, const QString &filePath)
{
    QMutexLocker locker(&m_documentsMutex);

    removeFileFromProjectInternal(projectName, filePath);
}

int ParsedDataStorage::getProjectsCount()
{
    QMutexLocker locker(&m_documentsMutex);

    return m_root->projects().size();
}

int ParsedDataStorage::getDocumentsCount()
{
    int cnt = 0;

    QMutexLocker locker(&m_documentsMutex);

    for (auto it = m_root->projects().begin();
         it != m_root->projects().end(); it++) {
        cnt += (*it)->files().size();
    }

    return cnt;
}

void ParsedDataStorage::removeFileFromProjectInternal(const QString &projectName, const QString &filePath)
{
    auto prj = m_root->project(projectName);
    if (prj == nullptr)
        return;

    prj->remove(filePath);
    m_typeReferences.removeReference(filePath);
}

QStringList ParsedDataStorage::getProjectsForFileInternal(const QString &filePath) const
{
    QStringList res;

    const auto &projects = m_root->projects();
    for (const auto &project : projects) {
        if (project->file(filePath) != nullptr)
            res.append(project->name());
    }

    return res;
}

const std::vector<std::shared_ptr<Data::File>> ParsedDataStorage::getFilesFromProjectInternal(const QString &projectName) const
{
    const auto project = m_root->project(projectName);
    if (project == nullptr)
        return std::vector<std::shared_ptr<Data::File>>();

    return project->files();
}

const Data::File *ParsedDataStorage::getFileForPathInternal(const QString &filePath) const
{
    for (auto it = m_root->projects().begin(); it != m_root->projects().end(); it++)
        if ((*it)->file(filePath) != nullptr)
            return (*it)->file(filePath);

    return nullptr;
}

Data::SourceLocation ParsedDataStorage::getLocationFromModule(const Data::Definitions &moduleDefinition,
                                                              const QString &typeAssignmentName) const
{
    const auto type = moduleDefinition.type(typeAssignmentName);
    if (!type)
        return {};
    return type->location();
}

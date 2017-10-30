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

#include <QObject>
#include <QHash>
#include <QMutex>
#include <QString>

#include <memory>

#include "data/root.h"
#include "data/file.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDataStorageProxy;

class ParsedDataStorage : public QObject
{
    Q_OBJECT

    ParsedDataStorage();
    ~ParsedDataStorage() = default;

    friend class ParsedDataStorageProxy;

public:
    static ParsedDataStorage *instance();

    const Data::Root *root() const { return m_root.get(); }

    const Data::File *getAnyFileForPath(const QString &filePath) const;
    Data::File *getFileForPathFromProject(const QString &project, const QString &path);

    const QStringList getProjectsForFile(const QString &filePath) const;
    const QStringList getFilesPathsFromProject(const QString &projectName) const;

    Data::SourceLocation getDefinitionLocation(const QString &path, const QString &typeAssignmentName, const QString &definitionsName) const;

signals:
    void fileUpdated(const QString &filePath, const Data::File *newFile);

private:
    void addProject(const QString &projectName);
    void removeProject(const QString &projectName);

    void addFileToProject(const QString &projectName, std::unique_ptr<Data::File> file);
    void removeFileFromProject(const QString &projectName, const QString &filePath);

    int getProjectsCount();
    int getDocumentsCount();

    const QStringList getProjectsForFileInternal(const QString &filePath) const;
    const QStringList getFilesPathsFromProjectInternal(const QString &projectName) const;
    void removeFileFromProjectInternal(const QString &projectName, const QString &filePath);

    const Data::File *getFileForPathInternal(const QString &filePath) const;

    Data::SourceLocation getLocationFromModule(const Data::Definitions &moduleDefinition,
                                               const QString &typeAssignmentName) const;

    std::unique_ptr<Data::Root> m_root;

    mutable QMutex m_documentsMutex;
};

} // namespace Internal
} // namespace Asn1Acn

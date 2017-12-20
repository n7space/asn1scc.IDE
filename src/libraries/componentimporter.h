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

#include <QString>
#include <QStringList>
#include <QDir>

namespace ProjectExplorer {
class Project;
}

namespace Asn1Acn {
namespace Internal {
namespace Libraries {

class ComponentImporter
{
public:
    void setDirectory(const QString &path);
    void setFiles(const QStringList &files);

    const QStringList &files() const;

    void import();

private:
    QStringList copyFilesToProject();
    QString targetFileName(const QString &file);
    void addFilesToProject(const ProjectExplorer::Project *project, const QStringList &files);
    QStringList createUniqueFilesList(const ProjectExplorer::Project *project, const QStringList &newFiles);

    QStringList m_sourceFiles;

    QDir m_sourceDir;
    QDir m_targetDir;
    QDir m_projectDir;
};

} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

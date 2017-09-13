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

#include "typeslocator.h"

#include <coreplugin/editormanager/editormanager.h>

#include <projectexplorer/session.h> // TODO: remove along with stubbed implementation
#include <projectexplorer/project.h> // TODO: remove along with stubbed implementation

using namespace Asn1Acn::Internal;

TypesLocator::TypesLocator()
{
    setShortcutString("asn");
    setDisplayName("ASN.1 type definitions");
    setId("ASN.1 type definitions");

    setIncludedByDefault(false);
}

void TypesLocator::accept(Entry selection) const
{
    Core::EditorManager::openEditorAt(selection.fileName, 1, 0);
}

void TypesLocator::refresh(QFutureInterface<void> &future)
{
    Q_UNUSED(future);
}

QList<TypesLocator::Entry> TypesLocator::matchesFor(QFutureInterface<Entry> &future, const QString &entry)
{    
    const QStringList files = createProjectList().filter(".asn").filter(entry);

    QList<Entry> ret;

    foreach (const auto &file, files) {
        if (future.isCanceled())
            break;

        Entry locatorEntry(this, file, QVariant());
        locatorEntry.fileName = file;

        ret.append(locatorEntry);
    }

    return ret;
}

QStringList TypesLocator::createProjectList()
{
    QList<ProjectExplorer::Project *> projectList =  ProjectExplorer::SessionManager::projects();
    if (projectList.empty())
        return QStringList();

    return projectList.at(0)->files(ProjectExplorer::Project::AllFiles);
}

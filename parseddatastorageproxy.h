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

#include <QString>

#include "parseddatastorage.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDataStorageProxy
{
protected:

    ParsedDataStorage *create()
    {
        return new ParsedDataStorage;
    }

    void finish(ParsedDataStorage *storage)
    {
        delete storage;
    }

    void addProject(ParsedDataStorage *storage, const QString &projectName)
    {
        storage->addProject(projectName);
    }

    void removeProject(ParsedDataStorage *storage, const QString &projectName)
    {
        storage->removeProject(projectName);
    }

    void addFileToProject(ParsedDataStorage *storage, const QString &projectName, std::unique_ptr<Data::File> file)
    {
        storage->addFileToProject(projectName, std::move(file));
    }

    void removeFileFromProject(ParsedDataStorage *storage, const QString &projectName, const QString &filePath)
    {
        storage->removeFileFromProject(projectName, filePath);
    }

    int getProjectsCount(ParsedDataStorage *storage)
    {
        return storage->getProjectsCount();
    }

    int getDocumentsCount(ParsedDataStorage *storage)
    {
        return storage->getDocumentsCount();
    }
};

} // namespace Internal
} // namespace Asn1Acn

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

#include "librarystorage.h"

#include <QMutexLocker>

using namespace Asn1Acn::Internal::Libraries;

LibraryStorage *LibraryStorage::instance()
{
    static LibraryStorage instance_;
    return &instance_;
}

void LibraryStorage::addLibrary(LibraryPtr library)
{
    QMutexLocker lock(&m_libraryMutex);

    emit aboutToChange();

    m_libraries[library->name()] = std::move(library);

    emit changed();
}

void LibraryStorage::addGeneralMetadata(Metadata::General metadata)
{
    QMutexLocker lock(&m_libraryMutex);
    m_generalMetadata[metadata.path()] = metadata;
}

Metadata::General LibraryStorage::generalMetadata(const QString &path)
{
    QMutexLocker lock(&m_libraryMutex);
    return m_generalMetadata[path];
}

void LibraryStorage::removeLibraries()
{
    QMutexLocker lock(&m_libraryMutex);

    emit aboutToChange();

    m_libraries.clear();

    emit changed();
}

void LibraryStorage::removeAllMetadata()
{
    QMutexLocker lock(&m_libraryMutex);
    m_generalMetadata.clear();
}

const Metadata::Library *LibraryStorage::library(const QString &path) const
{
    QMutexLocker lock(&m_libraryMutex);

    for (const auto &item : m_libraries)
        if (item.first == path)
            return item.second.get();

    return nullptr;
}

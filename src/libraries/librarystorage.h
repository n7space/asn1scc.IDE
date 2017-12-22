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
#include <map>

#include <QMutex>
#include <QString>

#include "metadata/library.h"
#include "metadata/general.h"

namespace Asn1Acn {
namespace Internal {
namespace Libraries {

class LibraryStorage : public QObject
{
    Q_OBJECT

    LibraryStorage() = default;
    ~LibraryStorage() = default;

public:
    using LibraryPtr = std::unique_ptr<Metadata::Library>;

    static LibraryStorage *instance();

    void addLibrary(LibraryPtr library);
    void removeLibraries();

    void addGeneralMetadata(Metadata::General metadata);
    void removeAllMetadata();

    Metadata::General generalMetadata(const QString &path);
    const Metadata::Library *library(const QString &path) const;

signals:
    void aboutToChange();
    void changed();

private:
    std::map<QString, LibraryPtr> m_libraries;
    std::map<QString, Metadata::General> m_generalMetadata;

    mutable QMutex m_libraryMutex;
};


} // namespace Libraries
} // namespace Internal
} // namespace Asn1Acn

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

#include "componentlibraryprocessor.h"

#include <memory>

#include "metadata/library.h"

#include "librarystorage.h"
#include "modulemetadataparser.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Libraries;

ComponentLibraryProcessor::ComponentLibraryProcessor(const QString &path,
                                                     const QStringList &files,
                                                     const FileSourceReader &reader,
                                                     QObject *parent)
    : QObject(parent)
    , m_reader(reader)
    , m_path(path)
    , m_files(files)
{
}

void ComponentLibraryProcessor::process()
{
    auto library = std::make_unique<Metadata::Library>(m_path);

    for (const auto &file : m_files) {
        const auto content = m_reader.readContent(file);
        ModuleMetadataParser parser(content.toLatin1());

        try {
            auto module = parser.parse();
            library->addModule(std::move(module));
        }
        catch (const ModuleMetadataParser::Error &err) {
            Q_UNUSED(err);
            // TODO: continue helps develop; in final implementation it should be return instead
            continue;
        }
    }

    LibraryStorage::instance()->addLibrary(std::move(library));

    this->deleteLater();
}

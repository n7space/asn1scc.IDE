/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include <utils/qtcassert.h>

#include <messages/messagemanager.h>

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
{}

void ComponentLibraryProcessor::process()
{
    auto library = std::make_unique<Metadata::Library>(m_path);

    for (const auto &file : m_files) {
        const auto content = m_reader.readContent(file);
        ModuleMetadataParser parser(content.toLatin1());

        try {
            auto module = parser.parse();
            library->addModule(std::move(module));
        } catch (const ModuleMetadataParser::Error &err) {
            using Messages::MessageManager;
            MessageManager::write(QLatin1String("Parsing failed - ") + err.what(),
                                  MessageManager::Type::Metadata);
            continue;
        }
    }

    LibraryStorage::instance()->addLibrary(std::move(library));

    this->deleteLater();
}

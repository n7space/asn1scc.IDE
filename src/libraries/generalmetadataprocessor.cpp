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

#include "generalmetadataprocessor.h"

#include "librarystorage.h"
#include "generalmetadataparser.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Libraries;

GeneralMetadataProcessor::GeneralMetadataProcessor(const QString &path,
                                                   const QString &file,
                                                   const FileSourceReader &reader,
                                                   QObject *parent)
    : QObject(parent)
    , m_reader(reader)
    , m_path(path)
    , m_file(file)
{
}

void GeneralMetadataProcessor::process()
{
    GeneralMetadataParser parser(m_reader.readContent(m_file).toLatin1(), m_path);
    LibraryStorage::instance()->addGeneralMetadata(parser.parse());

    this->deleteLater();
}

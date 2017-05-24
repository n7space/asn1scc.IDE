/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

#include "asnparseddatastorage.h"

using namespace Asn1Acn::Internal;

static AsnParsedDataStorage *m_instance;

AsnParsedDataStorage *AsnParsedDataStorage::instance()
{
    if (m_instance)
        return m_instance;

    m_instance = new AsnParsedDataStorage;
    return m_instance;
}

std::shared_ptr<AsnParsedDocument> AsnParsedDataStorage::getDataForFile(const QString &filePath)
const
{
    return m_items.contains(filePath) ? m_items.value(filePath) : nullptr;
}

void AsnParsedDataStorage::update(const QString &filePath,
                                  std::unique_ptr<AsnParsedDocument> newFile)
{
    if (m_items.contains(filePath) == true) {
        std::shared_ptr<AsnParsedDocument> oldFile = m_items.value(filePath);
        if (oldFile->getRevision() == newFile->getRevision())
            return;
    }

    m_items.insert(filePath, std::move(newFile));
}

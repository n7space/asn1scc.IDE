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

#include "parseddatastorage.h"

#include "utils/qtcassert.h"

using namespace Asn1Acn::Internal;

static ParsedDataStorage *m_instance;
static QMutex m_instanceMutex;

ParsedDataStorage *ParsedDataStorage::instance()
{
    if (m_instance)
        return m_instance;

    QMutexLocker locker(&m_instanceMutex);

    if (!m_instance)
        m_instance = new ParsedDataStorage;

    return m_instance;
}

std::shared_ptr<ParsedDocument>
ParsedDataStorage::getFileForPath(const QString &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);

    return m_documents.contains(filePath) ? m_documents.value(filePath) : nullptr;
}

void ParsedDataStorage::addFile(const QString &filePath, std::unique_ptr<ParsedDocument> file)
{
    std::shared_ptr<ParsedDocument> newFile = std::move(file);

    {
        QMutexLocker locker(&m_documentsMutex);

        if (m_documents.contains(filePath) == true) {
            std::shared_ptr<ParsedDocument> oldFile = m_documents.value(filePath);
            if (oldFile->getRevision() == newFile->getRevision())
                return;
        }

        m_documents.insert(filePath, newFile);
    }

    emit fileUpdated(filePath, newFile);
}

void ParsedDataStorage::removeFile(const QString &filePath)
{
    QMutexLocker locker(&m_documentsMutex);
    if (m_documents.contains(filePath) == false)
        return;

    m_documents.remove(filePath);
}

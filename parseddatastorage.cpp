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
ParsedDataStorage::getDataForFile(const QString &filePath) const
{
    QMutexLocker locker(&m_itemsMutex);

    return m_items.contains(filePath) ? m_items.value(filePath) : nullptr;
}

QList<std::shared_ptr<ParsedDocument> >
ParsedDataStorage::getAllParsedFiles() const
{
    QMutexLocker locker(&m_itemsMutex);

    return m_items.values();
}

void ParsedDataStorage::addFile(const QString &filePath, std::unique_ptr<ParsedDocument> newFile)
{
    QMutexLocker locker(&m_itemsMutex);

    if (m_items.contains(filePath) == true) {
        std::shared_ptr<ParsedDocument> oldFile = m_items.value(filePath);
        if (oldFile->getRevision() == newFile->getRevision())
            return;
    }

    m_items.insert(filePath, std::move(newFile));

    locker.unlock();

    emit storageUpdated();
}

void ParsedDataStorage::removeFile(const QString &filePath)
{
    QMutexLocker locker(&m_itemsMutex);
    if (m_items.contains(filePath) == false)
        return;

    m_items.remove(filePath);

    locker.unlock();

    emit storageUpdated();
}

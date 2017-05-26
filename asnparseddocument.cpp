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

#include "asnparseddocument.h"

using namespace Asn1Acn::Internal;

AsnParsedDocument::AsnParsedDocument() :
    m_filePath(QString()),
    m_revision(-1)
{
}

AsnParsedDocument::AsnParsedDocument(const QString &filePath, int revision,
                                     const QStringList &wordList) :
    m_filePath(filePath),
    m_revision(revision)
{
    foreach (const QString &entry, wordList)
        m_dataItems.push_back(new QVariant(entry));
}

AsnParsedDocument::~AsnParsedDocument()
{
    qDeleteAll(m_dataItems);
}

QVariant *AsnParsedDocument::at(int idx) const
{
    return idx < m_dataItems.size() ? m_dataItems[idx] : nullptr;
}

size_t AsnParsedDocument::getCount() const
{
    return m_dataItems.size();
}

int AsnParsedDocument::getRevision() const
{
    return m_revision;
}

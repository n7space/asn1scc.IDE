/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
** Contact: http://n7space.com
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
#include "sourcemapper.h"

#include <QFileInfo>

using namespace Asn1Acn::Internal;

SourceMapper::SourceMapper(const QHash<QString, QString> &documents)
{
    for (auto it = documents.begin(); it != documents.end(); it++)
        m_mapping.insert({ QFileInfo(it.key()).fileName(), it.key() });
}

QString SourceMapper::findByFileName(const QString &fileName) const
{
    const auto it = m_mapping.find(fileName);
    if (it == m_mapping.end())
        return fileName;
    return it->second;
}

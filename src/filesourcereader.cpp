/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
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

#include "filesourcereader.h"

#include <QFile>

#include <coreplugin/editormanager/documentmodel.h>
#include <coreplugin/idocument.h>

using namespace Asn1Acn::Internal;

FileSourceReader::FileSourceReader() {}

QString FileSourceReader::readContent(const QString &fileName) const
{
    const QString content = readFromDocumentModel(fileName);

    if (content.isEmpty())
        return readFromFile(fileName);

    return content;
}

QString FileSourceReader::readFromFile(const QString &fileName) const
{
    if (fileName.isEmpty())
        return QString();

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    return QString::fromLatin1(file.readAll());
}

QString FileSourceReader::readFromDocumentModel(const QString &fileName) const
{
    const auto document = Core::DocumentModel::documentForFilePath(
        Utils::FilePath::fromString(fileName));
    if (document == nullptr)
        return QString();

    return QString::fromLatin1(document->contents());
}

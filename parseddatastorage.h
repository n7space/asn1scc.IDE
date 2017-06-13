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

#pragma once

#include <QHash>
#include <QMutex>
#include <QString>

#include <memory>

#include "parseddocument.h"
#include "parsedtreenode.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDataStorage : public QObject
{
    Q_OBJECT

    ParsedDataStorage() = default;
    ~ParsedDataStorage() = default;

public:
    static ParsedDataStorage *instance();

    std::shared_ptr<ParsedDocument> getFileForPath(const QString &filePath) const;

    void addFile(const QString &filePath, std::unique_ptr<ParsedDocument> file);
    void removeFile(const QString &filePath);

signals:
    void fileUpdated(const QString &filePath, std::shared_ptr<ParsedDocument> newFile);

private:
    QHash<QString, std::shared_ptr<ParsedDocument>> m_documents;
    mutable QMutex m_documentsMutex;
};

} // namespace Internal
} // namespace Asn1Acn

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

#include <memory>

#include <QString>
#include <QFileInfo>
#include <QByteArray>

#include "parseddocument.h"
#include "asn1sccserviceprovider.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDocumentBuilder : public QObject
{
    Q_OBJECT

public:
    ParsedDocumentBuilder(const QString &documentData, const QFileInfo &fileInfo, int revision);
    std::unique_ptr<ParsedDocument> takeDocument();

signals:
    void finished();

private slots:
    void requestFinished();

private:
    void parseXML(const QByteArray &textData);

    QFileInfo m_fileInfo;
    int m_revision;

    std::unique_ptr<ParsedDocument> m_parsedDocument;
};

} /* namespace Internal */
} /* namespace Asn1Acn */

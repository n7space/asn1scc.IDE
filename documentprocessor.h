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

#include <QString>
#include <QByteArray>
#include <QTextDocument>

#include <memory>

#include "parseddocument.h"
#include "asn1sccserviceprovider.h"

namespace Asn1Acn {
namespace Internal {

class DocumentProcessor : public QObject
{
    Q_OBJECT
public:
    DocumentProcessor(const QTextDocument *doc, const QString &filePath, int revision);

    void run();

signals:
    void processingFinished() const;

private slots:
    void requestFinished();

private:
    std::unique_ptr<ParsedDocument> parseXML(const QByteArray &textData) const;
    void runFinished() const;

    const QTextDocument *m_textDocument;
    const QString m_filePath;
    const int m_revision;

    Asn1SccServiceProvider *m_serviceProvider;
};

} // namespace Internal
} // namespace Asn1Acn

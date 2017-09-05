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

#pragma once

#include <QObject>

#include <QSignalSpy>

#include "../documentprocessor.h"
#include "../asn1sccdocumentprocessor.h"

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class DocumentProcessorTests : public QObject
{
    Q_OBJECT

public:
    explicit DocumentProcessorTests(QObject *parent = 0);

private slots:
    void test_unstarted();
    void test_successful();
    void test_error();
    void test_failed();

private:
    void examine(DocumentProcessor *dp,
                 const QSignalSpy &spy,
                 const DocumentProcessor::State state,
                 const QString &fileName,
                 const QString &filePath) const;

    const QString m_projectName;

    const QString m_fileContent;
    const QString m_fileDir;

    Asn1SccDocumentProcessor::DocumentBuilderCreator m_docBuilderCreator;
};

} // namespace Tests
} // namespace Internal
} // namespace Asn1Acn

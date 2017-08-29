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

#include <QHash>
#include <QString>
#include <QFileInfo>
#include <QList>
#include <QTextDocument>

#include <memory>

#include "parseddocument.h"
#include "parseddocumentbuilderinterface.h"
#include "documentsourceinfo.h"

namespace Asn1Acn {
namespace Internal {

class DocumentProcessor : public QObject
{
    Q_OBJECT
public:

    enum class State {
        Unfinished,
        Successful,
        Failed,
        Errored
    };

    DocumentProcessor(const QString &projectName);
    ~DocumentProcessor();

    void addToRun(const QString &docContent, const QString &filePath, int revision);
    void run();
    std::vector<std::unique_ptr<ParsedDocument>> takeResults();

    State getState();

signals:
    void processingFinished(const QString &projectName) const;

private slots:
    void onBuilderFinished();
    void onBuilderFailed();
    void onBuilderErrored();

private:
    void createFallbackResults();

    QHash<QString, DocumentSourceInfo> m_documents;
    QString m_projectName;

    std::vector<std::unique_ptr<ParsedDocument>> m_results;
    State m_state;

    ParsedDocumentBuilderInterface *m_docBuilder;
};

} // namespace Internal
} // namespace Asn1Acn

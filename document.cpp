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

#include "document.h"

#include <QTextDocument>

#include <coreplugin/idocument.h>

#include "projectcontenthandler.h"

using namespace Asn1Acn::Internal;

static const int PROCESS_DOCUMENT_INTERVAL_MS = 1000;

Document::Document()
{
    m_processorTimer.setSingleShot(true);
    m_processorTimer.setInterval(PROCESS_DOCUMENT_INTERVAL_MS);

    connect(&m_processorTimer, &QTimer::timeout,
            this, &Document::processDocument, Qt::UniqueConnection);

    connect(this, &Core::IDocument::filePathChanged,
            this, &Document::onFilePathChanged);
}

void Document::scheduleProcessDocument()
{
    m_processorTimer.start();
}

void Document::onFilePathChanged(const Utils::FileName &oldPath, const Utils::FileName &newPath)
{
    if (newPath.isEmpty() || newPath == oldPath)
        return;

    connect(this, &Core::IDocument::contentsChanged,
            this, &Document::scheduleProcessDocument);
}

void Document::processDocument()
{
    QTextDocument *currentDocument = document();

    const QString path = filePath().toString();
    const QString content = currentDocument->toPlainText();
    const int revision = currentDocument->revision();

    ProjectContentHandler *proc = new ProjectContentHandler();
    proc->handleFileContentChanged(path, content, revision);
}

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

#include "asndocument.h"

#include "asnhighlighter.h"
#include "asn1acnconstants.h"
#include "asndocumentprocessor.h"

using namespace Asn1Acn::Internal;

const int PROCESS_DOCUMENT_INTERVAL_MS = 1000;

AsnDocument::AsnDocument()
{
    setId(Constants::ASNEDITOR_ID);
    setSyntaxHighlighter(new AsnHighlighter);

    m_processorTimer.setSingleShot(true);
    m_processorTimer.setInterval(PROCESS_DOCUMENT_INTERVAL_MS);

    connect(&m_processorTimer, &QTimer::timeout,
            this, &AsnDocument::processDocument, Qt::UniqueConnection);

    connect(this, &IDocument::filePathChanged,
            this, &AsnDocument::onFilePathChanged);

    /*
     * setSyntaxHighlighter(new CppHighlighter);
    setIndenter(new CppTools::CppQtStyleIndenter);

    connect(this, &TextEditor::TextDocument::tabSettingsChanged,
            this, &CppEditorDocument::invalidateFormatterCache);
    connect(this, &Core::IDocument::mimeTypeChanged,
            this, &CppEditorDocument::onMimeTypeChanged);

    connect(this, &Core::IDocument::aboutToReload,
            this, &CppEditorDocument::onAboutToReload);
    connect(this, &Core::IDocument::reloadFinished,
            this, &CppEditorDocument::onReloadFinished);
     * **/
}

void AsnDocument::scheduleProcessDocument()
{
    m_processorTimer.start();
}

void AsnDocument::onFilePathChanged(const Utils::FileName &oldPath, const Utils::FileName &newPath)
{
    if (newPath.isEmpty() || newPath == oldPath)
        return;

    connect(this, &Core::IDocument::contentsChanged, this, &AsnDocument::scheduleProcessDocument);
    scheduleProcessDocument();
}

void AsnDocument::processDocument()
{
    AsnDocumentProcessor docProcessor(*this);
    connect(&docProcessor, &AsnDocumentProcessor::asnDocumentUpdated,
            this, &AsnDocument::documentUpdated);
    docProcessor.run();
}

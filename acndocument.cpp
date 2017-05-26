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

#include "acndocument.h"

#include "asn1acnconstants.h"

using namespace Asn1Acn::Internal;

AcnDocument::AcnDocument()
{
    setId(Constants::ACNEDITOR_ID);

    /*
    setSyntaxHighlighter(new CppHighlighter);
    setIndenter(new CppTools::CppQtStyleIndenter);

    connect(this, &TextEditor::TextDocument::tabSettingsChanged,
            this, &CppEditorDocument::invalidateFormatterCache);
    connect(this, &Core::IDocument::mimeTypeChanged,
            this, &CppEditorDocument::onMimeTypeChanged);

    connect(this, &Core::IDocument::aboutToReload,
            this, &CppEditorDocument::onAboutToReload);
    connect(this, &Core::IDocument::reloadFinished,
            this, &CppEditorDocument::onReloadFinished);
    connect(this, &IDocument::filePathChanged,
            this, &CppEditorDocument::onFilePathChanged);
    */
}

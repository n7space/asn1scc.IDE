/****************************************************************************
**
** Copyright (C) 2017-2018 N7 Space sp. z o. o.
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

#pragma once

#include <QTextEdit>
#include <QTimer>

#include <texteditor/textdocument.h>

#include <utils/fileutils.h>

namespace Asn1Acn {
namespace Internal {

class Document : public TextEditor::TextDocument
{
    Q_OBJECT

public:
    explicit Document();

signals:
    void extraSelectionsUpdated(const QList<QTextEdit::ExtraSelection> &selections) const;

private slots:
    void onModelChanged();
    void onFileContentChanged();
    void onFilePathChanged(const Utils::FileName &oldPath, const Utils::FileName &newPath);

private:
    void scheduleProcessDocument();
    void processDocument();

    void updateExtraSelections() const;
    const QString activeProjectName() const;

    QTimer m_processorTimer;
};

} // namespace Internal
} // namespace Asn1Acn

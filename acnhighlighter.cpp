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

#include <QVector>
#include <QString>
#include <QStringList>
#include <QTextDocument>
#include <QRegularExpression>

#include "acnhighlighter.h"

using namespace Asn1Acn::Internal;
using namespace TextEditor;

AcnHighlighter::AcnHighlighter(QTextDocument *document) :
    Highlighter(document)
{
    static const QVector<TextStyle> categories({
        C_KEYWORD,
        C_COMMENT,
    });
    setTextFormatCategories(categories);

    QStringList keywords;
    keywords << "\\bsize\\b"
             << "\\bencoding\\b"
             << "\\bendianness\\b"
             << "\\balign-to-next\\b"
             << "\\bencode-values\\b"
             << "\\btrue-value\\b"
             << "\\bfalse-value\\b"
             << "\\bpresent-when\\b"
             << "\\bdeterminant\\b"
             << "\\bmapping-function\\b";

    foreach (const QString &pattern, keywords)
        m_keywordPatterns.append(QRegularExpression(pattern));
}

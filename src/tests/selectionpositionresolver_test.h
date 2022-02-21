/****************************************************************************
**
** Copyright (C) 2022 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include <QObject>

#include <selectionpositionresolver.h>
#include <texteditor/textdocument.h>

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class SelectionPositionResolverTests : public QObject
{
    Q_OBJECT

public:
    explicit SelectionPositionResolverTests(QObject *parent = 0);
    ~SelectionPositionResolverTests() = default;

private slots:
    void test_emptyDocument();
    void test_singleWord();
    void test_singleWordOnlyHyphens();

    void test_wordWithHyphenInMiddle();
    void test_wordWithMultipleHyhpens();
    void test_selectHyphensAsPartOfWord();

    void test_wordWithHyphensAtEnd();
    void test_selectHyphensAsWordEnding();

    void test_wordWithHyphensAtStart();
    void test_selectHyphensAsWordStarting();

    void test_wordPrecededWithHyphensOnlyWord();
    void test_wordFollowedByHyphensOnlyWord();

private:
    QTextCursor getInitializedCursor(const QByteArray &content, const int position);

    std::unique_ptr<TextEditor::TextDocument> m_doc;
    SelectionPositionResolver m_resolver;
};

} // namespace Tests
} // namespace Internal
} // namespace Asn1Acn

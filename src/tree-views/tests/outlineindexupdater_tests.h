/****************************************************************************
**
** Copyright (C) 2017 N7 Space sp. z o. o.
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

#include <QObject>
#include <QSignalSpy>

#include <data/node.h>

#include "../outlineindexupdater.h"

namespace Asn1Acn {
namespace Internal {
namespace TreeViews {
namespace Tests {

class OutlineIndexUpdaterTests : public QObject
{
    Q_OBJECT
public:
    explicit OutlineIndexUpdaterTests(QObject *parent = 0);
    ~OutlineIndexUpdaterTests();

private slots:
    void test_setEmptyEditor();
    void test_setNonEmpytEditorChangedPosition();

    void test_cursorMovedToModule();
    void test_cursorMovedToTypeDefinition();
    void test_cursorMovedToEmptyLine();
    void test_cursorMovedToSecondDefinitionInLine();

    void test_forceUpdate();
    void test_forceUpdateAfterCursorMoved();

    void test_removeEditorAfterLineUpdate();

private:
    Data::Node *createModelNodes(const QString &filePath);
    TextEditor::TextEditorWidget *createEditorWidget();
    ParsedDataStorage *createStorage();

    void verifySpyReceviedCorrectData(const QSignalSpy &spy);

    Data::Node *m_data;
    Model *m_model;
    OutlineIndexUpdater *m_indexUpdater;
    TextEditor::TextEditorWidget *m_editorWidget;

    ParsedDataStorage *m_storage;
};

} // namespace Tests
} // namespace TreeViews
} // namespace Internal
} // namespace Asn1Acn

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
#include "outlineindexupdater_tests.h"

#include <QtTest>

#include <QSignalSpy>
#include <QTest>

#include <texteditor/textdocument.h>

#include <data/definitions.h>
#include <data/file.h>
#include <data/typeassignment.h>
#include <data/types/userdefinedtype.h>

#include <parseddatastorage.h>

#include <tree-views/outlinemodel.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::TreeViews::Tests;

static const int RESPONSE_WAIT_MAX_TIME_MS = 20000;

OutlineIndexUpdaterTests::OutlineIndexUpdaterTests(QObject *parent)
    : QObject(parent)
{
    m_editorWidget = createEditorWidget();
    m_storage = createStorage();

    m_model = new OutlineModel({});
    m_model->setRoot(createModelNodes(m_editorWidget->textDocument()->filePath().toString()));

    m_indexUpdater = new OutlineIndexUpdater(m_model, m_storage, nullptr);
}

OutlineIndexUpdaterTests::~OutlineIndexUpdaterTests()
{
    delete m_indexUpdater;
    delete m_model;
    delete m_editorWidget;
    delete m_data;
    delete m_storage;
}

ParsedDataStorage *OutlineIndexUpdaterTests::createStorage()
{
    auto storage = new ParsedDataStorage();

    const QString projectName("projectName");
    const QString filePath = m_editorWidget->textDocument()->filePath().toString();

    storage->addProject(projectName);
    storage->addFileToProject(projectName, std::make_unique<Data::File>(filePath));

    return storage;
}

TextEditor::TextEditorWidget *OutlineIndexUpdaterTests::createEditorWidget()
{
    TextEditor::TextDocument *document = new TextEditor::TextDocument;
    Utils::FileName fileName = Utils::FileName::fromString("file1.asn1");
    document->setFilePath(fileName);

    // clang-format off
    document->setPlainText(
                 /*  1 */ "Test1 DEFINITIONS ::= BEGIN\n"
                 /*  2 */ "   Num1 ::= INTEGER (-100 .. 1000)\n"
                 /*  3 */ "   Num2 ::= INTEGER (0 .. 1000)\n"
                 /*  4 */ "END\n"
                 /*  5 */ "Test2 DEFINITIONS ::= BEGIN\n"
                 /*  6 */ "    Num3 ::= INTEGER (-100 .. 1000)\n"
                 /*  7 */ "    Num4 ::= INTEGER (0 .. 1000)\n"
                 /*  8 */ "    Num5 ::= INTEGER (0 .. 1000)           Num6 ::= INTEGER (0 .. 1000)\n"
                 /*  9 */ "END\n"
                 /* 10 */ "\n"
                 /* 11 */ "\n");
    // clang-format on

    QSharedPointer<TextEditor::TextDocument> documentPointer(document);

    TextEditor::TextEditorWidget *editorWidget = new TextEditor::TextEditorWidget;
    editorWidget->setTextDocument(documentPointer);

    return editorWidget;
}

Data::Node *OutlineIndexUpdaterTests::createModelNodes(const QString &filePath)
{
    const auto root = new Data::File(filePath);

    auto definitions1 = std::make_unique<Data::Definitions>("Test1",
                                                            Data::SourceLocation{filePath, 1, 0});
    definitions1->addType(std::make_unique<Data::TypeAssignment>(
        "Num1",
        Data::SourceLocation{filePath, 2, 3},
        std::make_unique<Data::Types::UserdefinedType>("UserTypeName", "Test1")));
    definitions1->addType(std::make_unique<Data::TypeAssignment>(
        "Num2",
        Data::SourceLocation{filePath, 3, 3},
        std::make_unique<Data::Types::UserdefinedType>("UserTypeName", "Test1")));
    root->add(std::move(definitions1));

    auto definitions2 = std::make_unique<Data::Definitions>("Test2",
                                                            Data::SourceLocation{filePath, 5, 0});
    definitions2->addType(std::make_unique<Data::TypeAssignment>(
        "Num3",
        Data::SourceLocation{filePath, 6, 3},
        std::make_unique<Data::Types::UserdefinedType>("UserTypeName", "Test2")));
    definitions2->addType(std::make_unique<Data::TypeAssignment>(
        "Num4",
        Data::SourceLocation{filePath, 7, 3},
        std::make_unique<Data::Types::UserdefinedType>("UserTypeName", "Test2")));

    definitions2->addType(std::make_unique<Data::TypeAssignment>(
        "Num5",
        Data::SourceLocation{filePath, 8, 3},
        std::make_unique<Data::Types::UserdefinedType>("UserTypeName", "Test2")));
    definitions2->addType(std::make_unique<Data::TypeAssignment>(
        "Num6",
        Data::SourceLocation{filePath, 8, 42},
        std::make_unique<Data::Types::UserdefinedType>("UserTypeName", "Test2")));
    root->add(std::move(definitions2));

    m_data = root;

    return m_data;
}

void OutlineIndexUpdaterTests::test_setEmptyEditor()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);

    m_indexUpdater->setEditor(nullptr);

    QCOMPARE(spy.count(), 1);

    const QVariant current = spy.at(0).at(1);
    QCOMPARE(current.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(current);
    QCOMPARE(index.isValid(), false);
}

void OutlineIndexUpdaterTests::test_setNonEmpytEditorChangedPosition()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);
    const int lineNumber = 2;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);
    verifySpyReceivedCorrectData(spy);

    const auto index = qvariant_cast<QModelIndex>(spy.at(0).at(1));
    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), lineNumber);
}

void OutlineIndexUpdaterTests::test_cursorMovedToModule()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);
    const int lineNumber = 1;
    const int columnNumber = 0;

    m_editorWidget->gotoLine(lineNumber, columnNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);
    verifySpyReceivedCorrectData(spy);

    const auto index = qvariant_cast<QModelIndex>(spy.at(0).at(1));
    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();

    QCOMPARE(location.line(), lineNumber);
    QCOMPARE(location.column(), columnNumber);
    QCOMPARE(node->name(), QString("Test1"));
}

void OutlineIndexUpdaterTests::test_cursorMovedToTypeDefinition()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);
    const int lineNumber = 3;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);
    verifySpyReceivedCorrectData(spy);

    const auto index = qvariant_cast<QModelIndex>(spy.at(0).at(1));
    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), lineNumber);
}

void OutlineIndexUpdaterTests::test_cursorMovedToEmptyLine()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);

    m_editorWidget->gotoLine(10);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);

    const QVariant current = spy.at(0).at(1);
    QCOMPARE(current.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(current);
    QCOMPARE(index.isValid(), false);
}

void OutlineIndexUpdaterTests::test_cursorMovedToSecondDefinitionInLine()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);
    const int lineNumber = 8;
    const int columnNumber = 45;

    m_editorWidget->gotoLine(lineNumber, columnNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);
    verifySpyReceivedCorrectData(spy);

    const auto index = qvariant_cast<QModelIndex>(spy.at(0).at(1));
    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), 8);
    QCOMPARE(location.column(), 42);
}

void OutlineIndexUpdaterTests::test_cursorMovedBetweenDefinitions()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);
    const int lineNumber = 8;
    const int columnNumber = 40;

    m_editorWidget->gotoLine(lineNumber, columnNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);
    verifySpyReceivedCorrectData(spy);

    const auto index = qvariant_cast<QModelIndex>(spy.at(0).at(1));
    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), 8);
    QCOMPARE(location.column(), 3);
}

void OutlineIndexUpdaterTests::test_forceUpdate()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);
    const int lineNumber = 6;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);
    m_indexUpdater->updateCurrentIndex();

    QCOMPARE(spy.count(), 1);
    verifySpyReceivedCorrectData(spy);

    const auto index = qvariant_cast<QModelIndex>(spy.at(0).at(1));
    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), lineNumber);
}

void OutlineIndexUpdaterTests::test_forceUpdateAfterCursorMoved()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);
    const int lineNumber = 7;

    m_indexUpdater->setEditor(m_editorWidget);
    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->updateCurrentIndex();

    QCOMPARE(spy.count(), 1);
    verifySpyReceivedCorrectData(spy);

    const auto index = qvariant_cast<QModelIndex>(spy.at(0).at(1));
    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), lineNumber);
}

void OutlineIndexUpdaterTests::test_removeEditorAfterLineUpdate()
{
    QSignalSpy spy(m_indexUpdater, &IndexUpdater::indexSelectionUpdated);
    const int lineNumber = 7;

    m_indexUpdater->setEditor(m_editorWidget);
    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(nullptr);

    QCOMPARE(spy.count(), 1);

    const QVariant current = spy.at(0).at(1);
    QCOMPARE(current.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(current);
    QCOMPARE(index.isValid(), false);
}

void OutlineIndexUpdaterTests::verifySpyReceivedCorrectData(const QSignalSpy &spy)
{
    QCOMPARE(spy.count(), 1);

    const QVariant current = spy.at(0).at(1);
    QCOMPARE(current.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(current);
    QCOMPARE(index.isValid(), true);

    const QModelIndexList list = qvariant_cast<QModelIndexList>(spy.at(0).at(0));
    QCOMPARE(list.size(), 1);
    QVERIFY(list.value(0) == index);
}

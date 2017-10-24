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
#include "outlineindexupdater_tests.h"

#include <QtTest>

#include <QTest>
#include <QSignalSpy>

#include <texteditor/textdocument.h>

#include <data/file.h>
#include <data/definitions.h>
#include <data/typeassignment.h>
#include <data/userdefinedtype.h>

#include <tree-views/outlinemodel.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::TreeViews::Tests;

static const int RESPONSE_WAIT_MAX_TIME_MS = 20000;

OutlineIndexUpdaterTests::OutlineIndexUpdaterTests(QObject *parent)
    : QObject(parent)
{
    m_editorWidget = createEditorWidget();

    m_model = new OutlineModel;
    m_model->setRoot(createModelNodes(m_editorWidget->textDocument()->filePath().toString()));

    m_indexUpdater = new OutlineIndexUpdater(m_model, nullptr);
}

OutlineIndexUpdaterTests::~OutlineIndexUpdaterTests()
{
    delete m_indexUpdater;
    delete m_model;
    delete m_editorWidget;
    delete m_data;
}

TextEditor::TextEditorWidget *OutlineIndexUpdaterTests::createEditorWidget()
{
    TextEditor::TextDocument *document = new TextEditor::TextDocument;
    document->setPlainText("Test1 DEFINITIONS ::= BEGIN\n"
                           "   Num1 ::= INTEGER (-100 .. 1000)\n"
                           "   Num2 ::= INTEGER (0 .. 1000)\n"
                           "END\n"
                           "Test2 DEFINITIONS ::= BEGIN\n"
                           "    Num3 ::= INTEGER (-100 .. 1000)\n"
                           "    Num4 ::= INTEGER (0 .. 1000)\n"
                           "END\n"
                           "\n"
                           "\n");

    Utils::FileName fileName = Utils::FileName::fromString("file1.asn1");
    document->setFilePath(fileName);

    QSharedPointer<TextEditor::TextDocument> documentPointer(document);

    TextEditor::TextEditorWidget *editorWidget = new TextEditor::TextEditorWidget;
    editorWidget->setTextDocument(documentPointer);

    return editorWidget;
}

Data::Node *OutlineIndexUpdaterTests::createModelNodes(const QString &filePath)
{
    const auto root = new Data::File(Data::Source(filePath, "CONTENTS"));

    auto definitions1 = std::make_unique<Data::Definitions>("Module1", Data::SourceLocation{filePath, 0, 0});
    definitions1->add(std::make_unique<Data::TypeAssignment>("Num1", Data::SourceLocation{filePath, 2, 3}, std::make_unique<Data::UserdefinedType>("UserTypeName", "Module1")));
    definitions1->add(std::make_unique<Data::TypeAssignment>("Num2", Data::SourceLocation{filePath, 3, 3}, std::make_unique<Data::UserdefinedType>("UserTypeName", "Module1")));
    root->add(std::move(definitions1));

    auto definitions2 = std::make_unique<Data::Definitions>("Module2", Data::SourceLocation{filePath, 5, 0});
    definitions2->add(std::make_unique<Data::TypeAssignment>("Num3", Data::SourceLocation{filePath, 6, 3}, std::make_unique<Data::UserdefinedType>("UserTypeName", "Module2")));
    definitions2->add(std::make_unique<Data::TypeAssignment>("Num4", Data::SourceLocation{filePath, 7, 3}, std::make_unique<Data::UserdefinedType>("UserTypeName", "Module2")));
    root->add(std::move(definitions2));

    m_data = root;
    return m_data;
}

void OutlineIndexUpdaterTests::test_setEmptyEditor()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);

    m_indexUpdater->setEditor(nullptr);

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

void OutlineIndexUpdaterTests::test_setNonEmpytEditorInitialCursorPosition()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);

    m_editorWidget->gotoLine(0);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

void OutlineIndexUpdaterTests::test_setNonEmpytEditorChangedPosition()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);
    const int lineNumber = 2;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), true);

    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), lineNumber);
}

void OutlineIndexUpdaterTests::test_cursorMovedToModule()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);

    m_editorWidget->gotoLine(1);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

void OutlineIndexUpdaterTests::test_cursorMovedToTypeDefinition()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);
    const int lineNumber = 3;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), true);

    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), lineNumber);
}

void OutlineIndexUpdaterTests::test_cursorMovedToEmptyLine()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);

    m_editorWidget->gotoLine(8);
    m_indexUpdater->setEditor(m_editorWidget);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

void OutlineIndexUpdaterTests::test_forceUpdate()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);
    const int lineNumber = 6;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);
    m_indexUpdater->updateCurrentIndex();

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), true);

    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), lineNumber);
}

void OutlineIndexUpdaterTests::test_forceUpdateAfterCursorMoved()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);
    const int lineNumber = 7;

    m_indexUpdater->setEditor(m_editorWidget);
    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->updateCurrentIndex();

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), true);

    const auto node = m_model->dataNode(index);
    const Data::SourceLocation location = node->location();
    QCOMPARE(location.line(), lineNumber);
}

void OutlineIndexUpdaterTests::test_removeEditorAfterLineUpdate()
{
    QSignalSpy spy(m_indexUpdater, &OutlineIndexUpdater::currentIndexUpdated);
    const int lineNumber = 7;

    m_indexUpdater->setEditor(m_editorWidget);
    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(nullptr);

    QVERIFY(spy.wait(RESPONSE_WAIT_MAX_TIME_MS));

    QCOMPARE(spy.count(), 2);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

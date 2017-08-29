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

#include "overviewindexupdater_tests.h"

#include <QtTest>

#include <QTest>
#include <QSignalSpy>

#include <texteditor/textdocument.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

static const int RESPONSE_WAIT_TIME_MS = 600;

OverviewIndexUpdaterTests::OverviewIndexUpdaterTests(QObject *parent)
    : QObject(parent)
{
    m_model = new OverviewModel;
    m_model->setRootNode(createModelNodes());

    m_indexUpdater = new OverviewIndexUpdater(m_model);

    m_editorWidget = createEditorWidget();
}

OverviewIndexUpdaterTests::~OverviewIndexUpdaterTests()
{
    delete m_indexUpdater;
    delete m_model;
    delete m_editorWidget;
}

TextEditor::TextEditorWidget *OverviewIndexUpdaterTests::createEditorWidget()
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

    QSharedPointer<TextEditor::TextDocument> documentPointer(document);

    TextEditor::TextEditorWidget *editorWidget = new TextEditor::TextEditorWidget;
    editorWidget->setTextDocument(documentPointer);

    return editorWidget;
}

ModelTreeNode::ModelTreeNodePtr OverviewIndexUpdaterTests::createModelNodes()
{
    ModelTreeNode::ModelTreeNodePtr rootNode = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode);

    Data::SourceLocation location = Data::SourceLocation("", 0, 0);
    ModelTreeNode::ModelTreeNodePtr moduleNode = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode("Module1", location));

    location = Data::SourceLocation("", 2, 3);
    moduleNode->addChild(ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode("Num1", location)));
    location = Data::SourceLocation("", 3, 3);
    moduleNode->addChild(ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode("Num2", location)));
    rootNode->addChild(moduleNode);

    location = Data::SourceLocation("", 0, 0);
    moduleNode = ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode("Module2", location));

    location = Data::SourceLocation("", 6, 3);
    moduleNode->addChild(ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode("Num3", location)));
    location = Data::SourceLocation("", 7, 3);
    moduleNode->addChild(ModelTreeNode::ModelTreeNodePtr(new ModelTreeNode("Num4", location)));
    rootNode->addChild(moduleNode);

    return rootNode;
}

void OverviewIndexUpdaterTests::test_setEmptyEditor()
{
    QSignalSpy spy(m_indexUpdater, &OverviewIndexUpdater::currentIndexUpdated);

    m_indexUpdater->setEditor(nullptr);

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

void OverviewIndexUpdaterTests::test_setNonEmpytEditorInitialCursorPosition()
{
    QSignalSpy spy(m_indexUpdater, &OverviewIndexUpdater::currentIndexUpdated);

    m_editorWidget->gotoLine(0);
    m_indexUpdater->setEditor(m_editorWidget);

    QTest::qWait(RESPONSE_WAIT_TIME_MS);

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

void OverviewIndexUpdaterTests::test_setNonEmpytEditorChangedPosition()
{
    QSignalSpy spy(m_indexUpdater, &OverviewIndexUpdater::currentIndexUpdated);
    const int lineNumber = 2;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QTest::qWait(RESPONSE_WAIT_TIME_MS);

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), true);

    const ModelTreeNode *symbol = static_cast<ModelTreeNode *>(index.internalPointer());
    const Data::SourceLocation location = symbol->getSourceLocation();
    QCOMPARE(location.line(), lineNumber);
}

void OverviewIndexUpdaterTests::test_cursorMovedToModule()
{
    QSignalSpy spy(m_indexUpdater, &OverviewIndexUpdater::currentIndexUpdated);

    m_editorWidget->gotoLine(1);
    m_indexUpdater->setEditor(m_editorWidget);

    QTest::qWait(RESPONSE_WAIT_TIME_MS);

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

void OverviewIndexUpdaterTests::test_cursorMovedToTypeDefinition()
{
    QSignalSpy spy(m_indexUpdater, &OverviewIndexUpdater::currentIndexUpdated);
    const int lineNumber = 3;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);

    QTest::qWait(RESPONSE_WAIT_TIME_MS);

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), true);

    const ModelTreeNode *symbol = static_cast<ModelTreeNode *>(index.internalPointer());
    const Data::SourceLocation location = symbol->getSourceLocation();
    QCOMPARE(location.line(), lineNumber);
}

void OverviewIndexUpdaterTests::test_cursorMovedToEmptyLine()
{
    QSignalSpy spy(m_indexUpdater, &OverviewIndexUpdater::currentIndexUpdated);

    m_editorWidget->gotoLine(8);
    m_indexUpdater->setEditor(m_editorWidget);

    QTest::qWait(RESPONSE_WAIT_TIME_MS);

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), false);
}

void OverviewIndexUpdaterTests::test_forceUpdate()
{
    QSignalSpy spy(m_indexUpdater, &OverviewIndexUpdater::currentIndexUpdated);
    const int lineNumber = 6;

    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->setEditor(m_editorWidget);
    m_indexUpdater->updateCurrentIndex();

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), true);

    const ModelTreeNode *symbol = static_cast<ModelTreeNode *>(index.internalPointer());
    const Data::SourceLocation location = symbol->getSourceLocation();
    QCOMPARE(location.line(), lineNumber);
}

void OverviewIndexUpdaterTests::test_forceUpdateAfterCursorMoved()
{
    QSignalSpy spy(m_indexUpdater, &OverviewIndexUpdater::currentIndexUpdated);
    const int lineNumber = 7;

    m_indexUpdater->setEditor(m_editorWidget);
    m_editorWidget->gotoLine(lineNumber);
    m_indexUpdater->updateCurrentIndex();

    QTest::qWait(RESPONSE_WAIT_TIME_MS);

    QCOMPARE(spy.count(), 1);

    const QVariant result = spy.at(0).at(0);
    QCOMPARE(result.type(), QVariant::ModelIndex);

    const QModelIndex index = qvariant_cast<QModelIndex>(result);
    QCOMPARE(index.isValid(), true);

    const ModelTreeNode *symbol = static_cast<ModelTreeNode *>(index.internalPointer());
    const Data::SourceLocation location = symbol->getSourceLocation();
    QCOMPARE(location.line(), lineNumber);
}

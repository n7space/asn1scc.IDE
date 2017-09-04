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

#include "structuresviewindexupdater_tests.h"

#include <QtTest>

#include <QTest>
#include <QSignalSpy>

#include <texteditor/textdocument.h>
#include <utils/fileutils.h>

#include <QDebug>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

static const int RESPONSE_WAIT_TIME_MS = 600;
static const QString FILE_PATH("/test/file1.asn");

StructuresViewIndexUpdaterTests::StructuresViewIndexUpdaterTests(QObject *parent)
    : QObject(parent)
{
    m_model = new OverviewModel;
    m_model->setRootNode(createModelNodes());

    m_indexUpdater = new StructuresViewIndexUpdater(m_model);

    m_editorWidget = createEditorWidget();
}

StructuresViewIndexUpdaterTests::~StructuresViewIndexUpdaterTests()
{
    delete m_indexUpdater;
    delete m_model;
    delete m_editorWidget;
}

TextEditor::TextEditorWidget *StructuresViewIndexUpdaterTests::createEditorWidget()
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

    document->setFilePath(Utils::FileName::fromString(FILE_PATH));

    QSharedPointer<TextEditor::TextDocument> documentPointer(document);

    TextEditor::TextEditorWidget *editorWidget = new TextEditor::TextEditorWidget;
    editorWidget->setTextDocument(documentPointer);

    return editorWidget;
}

ModelTreeNode::ModelTreeNodePtr StructuresViewIndexUpdaterTests::createModelNodes()
{
    auto rootNode = ModelTreeNode::makePtr();
    auto projectNode = ModelTreeNode::makePtr();
    rootNode->addChild(projectNode);

    Data::SourceLocation location = Data::SourceLocation(FILE_PATH, 0, 0);
    auto fileNode = ModelTreeNode::makePtr(FILE_PATH, Data::Type::UserDefined, location);
    projectNode->addChild(fileNode);

    location = Data::SourceLocation(FILE_PATH, 0, 0);
    auto moduleNode = ModelTreeNode::makePtr("Module1", Data::Type::UserDefined, location);

    location = Data::SourceLocation(FILE_PATH, 2, 3);
    moduleNode->addChild(ModelTreeNode::makePtr("Num1", Data::Type::Integer, location));
    location = Data::SourceLocation(FILE_PATH, 3, 3);
    moduleNode->addChild(ModelTreeNode::makePtr("Num2", Data::Type::Integer, location));
    fileNode->addChild(moduleNode);

    location = Data::SourceLocation(FILE_PATH, 0, 0);
    moduleNode = ModelTreeNode::makePtr("Module2", Data::Type::UserDefined, location);

    location = Data::SourceLocation(FILE_PATH, 6, 3);
    moduleNode->addChild(ModelTreeNode::makePtr("Num3", Data::Type::Integer, location));
    location = Data::SourceLocation(FILE_PATH, 7, 3);
    moduleNode->addChild(ModelTreeNode::makePtr("Num4", Data::Type::Integer, location));
    fileNode->addChild(moduleNode);

    return rootNode;
}

void StructuresViewIndexUpdaterTests::test_forceUpdate()
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
    const Data::SourceLocation location = symbol->sourceLocation();
    QCOMPARE(location.path(), FILE_PATH);
    QCOMPARE(location.line(), lineNumber);
}

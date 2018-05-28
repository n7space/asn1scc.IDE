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

#include "linkcreator_tests.h"

#include <QtTest>

#include <QTextCursor>

#include <texteditor/textdocument.h>
#include <utils/fileutils.h>

#include <data/file.h>
#include <data/typereference.h>
#include <data/types/builtintypes.h>

#include <linkcreator.h>
#include <parseddatastorage.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

LinkCreatorTests::LinkCreatorTests(QObject *parent)
    : QObject(parent)
    , m_storage(std::make_unique<ParsedDataStorage>())
    , m_path("/path/to/file/file.asn")
    , m_project("project_name")
{
    m_storage->addProject(m_project);
}

namespace {
std::unique_ptr<TextEditor::TextDocument> createDocument(const QString &path,
                                                         const QByteArray &content)
{
    auto doc = std::make_unique<TextEditor::TextDocument>();
    doc->setFilePath(Utils::FileName::fromString(path));
    doc->setContents(content);

    return doc;
}
} // namespace

void LinkCreatorTests::test_createLinksInEmptyDocument()
{
    auto doc = createDocument(m_path, "          ");

    QTextCursor cursor(doc->document());
    cursor.setPosition(5);

    LinkCreator linkCreator(*doc, m_storage.get());

    const auto highlightLink = linkCreator.createHighlightLink(cursor);
    QVERIFY(!highlightLink.hasValidLinkText());
    QVERIFY(highlightLink.hasValidTarget());
    QCOMPARE(highlightLink.targetFileName, m_path);

    const auto targetLink = linkCreator.createTargetLink(cursor);
    QVERIFY(!targetLink.hasValidLinkText());
    QVERIFY(!targetLink.hasValidTarget());
    QCOMPARE(targetLink.targetFileName, QString());
}

void LinkCreatorTests::test_createLinksFromEmptyStorage()
{
    auto doc = createDocument(m_path, "TAILORED-PUS-C DEFINITIONS AUTOMATIC TAGS ::= BEGIN \nEND");

    QTextCursor cursor(doc->document());
    cursor.setPosition(13);

    LinkCreator linkCreator(*doc, m_storage.get());

    const auto highlightLink = linkCreator.createHighlightLink(cursor);
    QVERIFY(!highlightLink.hasValidLinkText());
    QVERIFY(highlightLink.hasValidTarget());
    QCOMPARE(highlightLink.targetFileName, m_path);

    const auto targetLink = linkCreator.createTargetLink(cursor);
    QVERIFY(!targetLink.hasValidLinkText());
    QVERIFY(!targetLink.hasValidTarget());
    QCOMPARE(targetLink.targetFileName, QString());
}

void LinkCreatorTests::test_createHighlight()
{
    auto file = std::make_unique<Data::File>(m_path);
    auto ref = std::make_unique<Data::TypeReference>("TypeName",
                                                     "ModuleName",
                                                     Data::SourceLocation(m_path, 1, 5));

    file->addTypeReference(std::move(ref));

    m_storage->addFileToProject(m_project, std::move(file));

    auto doc = createDocument(m_path, "TAILORED-PUS-C DEFINITIONS AUTOMATIC TAGS ::= BEGIN \nEND");
    QTextCursor cursor(doc->document());
    cursor.setPosition(10);

    LinkCreator linkCreator(*doc, m_storage.get());
    const auto link = linkCreator.createHighlightLink(cursor);

    QVERIFY(link.hasValidLinkText());
    QCOMPARE(link.linkTextStart, 5);
    QCOMPARE(link.linkTextEnd, 13);
    QCOMPARE(link.targetFileName, m_path);

    m_storage->removeFileFromProject(m_project, Utils::FileName::fromString(m_path));
}

void LinkCreatorTests::test_createTarget()
{
    auto file = std::make_unique<Data::File>(m_path);
    auto ref = std::make_unique<Data::TypeReference>("TypeName",
                                                     "ModuleName",
                                                     Data::SourceLocation(m_path, 1, 5));

    file->addTypeReference(std::move(ref));

    auto typeAssignment
        = std::make_unique<Data::TypeAssignment>("TypeName",
                                                 Data::SourceLocation(m_path, 1, 15),
                                                 std::make_unique<Data::Types::Integer>());
    auto defs = std::make_unique<Data::Definitions>("ModuleName",
                                                    Data::SourceLocation(m_path, 0, 0));

    defs->addType(std::move(typeAssignment));
    file->add(std::move(defs));
    m_storage->addFileToProject(m_project, std::move(file));

    auto doc = createDocument(m_path, "TAILORED-PUS-C DEFINITIONS AUTOMATIC TAGS ::= BEGIN \nEND");
    QTextCursor cursor(doc->document());
    cursor.setPosition(10);

    LinkCreator linkCreator(*doc, m_storage.get());
    const auto link = linkCreator.createTargetLink(cursor);

    QVERIFY(link.hasValidTarget());
    QCOMPARE(link.targetColumn, 15);
    QCOMPARE(link.targetLine, 1);
    QCOMPARE(link.targetFileName, m_path);

    m_storage->removeFileFromProject(m_project, Utils::FileName::fromString(m_path));
}

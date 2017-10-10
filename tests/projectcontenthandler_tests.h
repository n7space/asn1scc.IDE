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

#pragma once

#include <QObject>

#include <documentprocessor.h>
#include <parseddatastorage.h>

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class ProjectContentHandlerTests : public QObject
{
    Q_OBJECT

public:
    explicit ProjectContentHandlerTests(QObject *parent = 0);

private slots:
    void test_singleProjectAddedAndRemoved();
    void test_multipleProjectsAddedAndRemoved();
    void test_projectAddedAndRemovedTwice();

    void test_singleFileAddedAndRemoved();
    void test_multipleFilesAddedAndRemoved();
    void test_fileAddedToMultipleProjects();

    void test_fileContentChanged();
    void test_fileContentChangedNoProject();
    void test_fileInMultipleProjectContentChanged();

private:
    void addProject(const QString &projectName);
    void removeProject(const QString &projectName);

    void fileListChanged(const QString &projectName, const QStringList &files);
    void fileContentChanged(const QString &path, const QString &content);

    ParsedDataStorage *m_storage;

    QStringList m_fileTypes;

    std::function<DocumentProcessor *(const QString &)> m_createProcessor;
};

} // namespace Tests
} // namespace Internal
} // namespace Asn1Acn

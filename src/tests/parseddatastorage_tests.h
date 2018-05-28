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

#include <parseddatastorage.h>

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class ParsedDataStorageTests : public QObject
{
    Q_OBJECT

public:
    explicit ParsedDataStorageTests(QObject *parent = 0);

private slots:
    void test_addAndRemoveProject();
    void test_addAndRemoveMultipleProjects();

    void test_addAndRemoveFileFromProject();
    void test_addAndRemoveMultipleFilesFromProject();

    void test_removeNonemptyProject();

    void test_updateFileInOneProject();
    void test_updateFileInMultipleProjects();

    void test_getFilesFromNonExistingProject();

private:
    void addFileToProject(ParsedDataStorage *storage,
                          const QString &project,
                          const Utils::FileName &filePath);

    Utils::FileName pathFromName(const QString &name);
};

} // namespace Tests
} // namespace Internal
} // namespace Asn1Acn

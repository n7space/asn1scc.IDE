/****************************************************************************
**
** Copyright (C) 2018 N7 Space sp. z o. o.
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

#include "testgeneratorparamsdialog.h"

#include "testgeneratorparamswidget.h"

#include "parseddatastorage.h"

#include <projectexplorer/project.h>
#include <projectexplorer/session.h>

#include <QDebug>

using namespace Asn1Acn::Internal::TestGenerator;

TestGeneratorParamsDialog::TestGeneratorParamsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_widget = new TestGeneratorWidget(this);

    connect(m_widget->buttonBox(),
            &QDialogButtonBox::accepted,
            this,
            &TestGeneratorParamsDialog::accept);

    connect(m_widget->buttonBox(), &QDialogButtonBox::rejected, this, &QDialog::reject);
}

int TestGeneratorParamsDialog::exec()
{
    m_widget->clearMainStructCandidates();
    m_widget->setPath(QStringLiteral(""));

    fillMainStructCandidates();

    return QDialog::exec();
}

void TestGeneratorParamsDialog::accept()
{
    // TODO: Add actual running of MalTester here
    QDialog::accept();
}

namespace {
QString projectName()
{
    const auto &project = ProjectExplorer::SessionManager::startupProject();
    return project == nullptr ? QString() : project->displayName();
}
} // namespace

void TestGeneratorParamsDialog::fillMainStructCandidates()
{
    fillMainStructCandidatesFromProject(projectName());
}

void TestGeneratorParamsDialog::fillMainStructCandidatesFromProject(const QString &projectName)
{
    auto storage = ParsedDataStorage::instance();
    const auto &paths = storage->getFilesPathsFromProject(projectName);
    for (const auto &path : paths) {
        const auto &file = storage->getAnyFileForPath(path);
        if (!file)
            continue;

        fillMainStructCandidatesFromDefinitions(file->definitionsList());
    }
}

void TestGeneratorParamsDialog::fillMainStructCandidatesFromDefinitions(
    const Data::File::DefinitionsList &defs)
{
    for (const auto &def : defs) {
        auto &types = def->types();
        for (const auto &type : types)
            m_widget->addMainStructCandidate(type->name());
    }
}

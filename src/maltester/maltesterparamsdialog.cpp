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
#include <QPushButton>

#include <projectexplorer/project.h>
#include <projectexplorer/session.h>

#include "maltesterparamsdialog.h"
#include "maltesterparamswidget.h"

#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::MalTester;

MalTesterParamsDialog::MalTesterParamsDialog(MalTesterParamsProviderPtr provider, QWidget *parent)
    : QDialog(parent)
    , m_provider(provider)
{
    m_widget = new MalTesterParamsWidget(this);
    m_runner = std::make_unique<MalTesterRunner>(provider);

    connect(m_widget->buttonBox(),
            &QDialogButtonBox::accepted,
            this,
            &MalTesterParamsDialog::accept);

    connect(m_widget->buttonBox(), &QDialogButtonBox::rejected, this, &QDialog::reject);
}

int MalTesterParamsDialog::exec()
{
    m_widget->setPath(
        ProjectExplorer::SessionManager::startupProject()->projectDirectory().toString());

    m_widget->clearRootTypeCandidates();
    fillRootTypeCandidates();
    letProceed(m_widget->comboBox()->count() > 0);

    return QDialog::exec();
}

void MalTesterParamsDialog::accept()
{
    m_provider->setRootTypeName(m_widget->rootType());
    m_provider->setOutputPath(m_widget->path());

    m_runner->run();

    QDialog::accept();
}

namespace {
QString projectName()
{
    const auto &project = ProjectExplorer::SessionManager::startupProject();
    return project == nullptr ? QString() : project->displayName();
}
} // namespace

void MalTesterParamsDialog::fillRootTypeCandidates()
{
    fillRootTypeCandidatesFromProject(projectName());
}

void MalTesterParamsDialog::fillRootTypeCandidatesFromProject(const QString &projectName)
{
    auto storage = ParsedDataStorage::instance();
    const auto &paths = storage->getFilesPathsFromProject(projectName);
    for (const auto &path : paths) {
        const auto &file = storage->getAnyFileForPath(path);
        if (!file)
            continue;

        fillRootTypeCandidatesFromDefinitions(file->definitionsList());
    }
}

void MalTesterParamsDialog::fillRootTypeCandidatesFromDefinitions(
    const Data::File::DefinitionsList &defs)
{
    for (const auto &def : defs) {
        auto &types = def->types();
        for (const auto &type : types)
            m_widget->addRootTypeCandidate(def->name() + "." + type->name());
    }
}

void MalTesterParamsDialog::letProceed(bool val)
{
    m_widget->comboBox()->setEnabled(val);
    m_widget->buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(val);
}

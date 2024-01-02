/****************************************************************************
**
** Copyright (C) 2017-2024 N7 Space sp. z o. o.
** Contact: https://n7space.com
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

#include "asn1acn_global.h"

#include <extensionsystem/iplugin.h>

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/icontext.h>

#include "libraries/wizard/importcomponentwizard.h"

#include "fuzzer/fuzzerparamsdialog.h"
#include "fuzzer/fuzzerparamsprovider.h"

namespace Asn1Acn {
namespace Internal {

namespace Icd {
class IcdBuilder;
}

class Asn1AcnPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Asn1Acn.json")

public:
    Asn1AcnPlugin();
    ~Asn1AcnPlugin() override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private slots:
    void raiseImportComponentWindow(ProjectExplorer::Project *project);
    void findUsages();
    void onTaskStarted(Utils::Id id);
    void onTaskFinished(Utils::Id id);

private:
    using ActionContainer = Core::ActionContainer;
    using Context = Core::Context;

    void initializeMenus(Settings::FuzzerConstPtr fuzzerSettings);

    void initializeSwitchAction(ActionContainer *toolsMenu,
                                ActionContainer *contextMenu,
                                const Context &context);
    void initializeFollowSymbolAction(ActionContainer *toolsMenu, ActionContainer *contextMenu);
    void initializeOpenInNextSplitAction(ActionContainer *toolsMenu, const Context &context);
    void initializeImportFromAsnComponents(ActionContainer *toolsMenu);
    void initializeFindUsagesAction(ActionContainer *toolsMenu,
                                    ActionContainer *contextMenu,
                                    const Context &context);
    void initializeBuildICDAction(ActionContainer *toolsMenu);
    void initializeGenerateTestsAction(ActionContainer *toolsMenu,
                                       Settings::FuzzerConstPtr settings);

    void addBuildICDToToolsMenu(ActionContainer *toolsMenu);
    void addBuildICDToProjectMenu();

    void addImportFromAsnComponentsToToolsMenu(ActionContainer *toolsMenu);
    void addImportFromAsnComponentsToProjectMenu();

    void addToToolsMenu(ActionContainer *container);

    QPointer<Libraries::Wizard::ImportComponentWizard> m_importComponentWizard;

    QPointer<Fuzzer::FuzzerParamsDialog> m_fuzzerDialog;

    QAction *m_findUsagesAction;

    std::shared_ptr<Icd::IcdBuilder> m_builder;

#ifdef WITH_TESTS
    QVector<QObject *> createTestObjects() const override;
#endif

    class Asn1AcnPluginPrivate *d = nullptr;
};

} // namespace Internal
} // namespace Asn1Acn

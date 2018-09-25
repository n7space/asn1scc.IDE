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
#include "asn1acn.h"

#include <QMenu>

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/jsexpander.h>
#include <coreplugin/progressmanager/progressmanager.h>

#include <extensionsystem/pluginmanager.h>

#include <texteditor/texteditorconstants.h>

#include <projectexplorer/kitmanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projecttree.h>
#include <projectexplorer/session.h>

#include <utils/parameteraction.h>

#include "completion/acnsnippets.h"
#include "completion/asnsnippets.h"

#include "settings/fuzzer.h"
#include "settings/libraries.h"
#include "settings/service.h"
#include "settings/settings.h"

#include "options-pages/fuzzer.h"
#include "options-pages/libraries.h"
#include "options-pages/service.h"

#include "tree-views/outlinewidget.h"
#include "tree-views/typestreewidget.h"

#include "libraries/componentdirectorywatcher.h"
#include "libraries/componentlibrarydispatcher.h"

#include "fuzzer/fuzzerparamsdialog.h"

#include "acneditor.h"
#include "asn1acnconstants.h"
#include "asn1acnjsextension.h"
#include "asn1sccserviceprovider.h"
#include "asneditor.h"
#include "icdbuilder.h"
#include "kitinformation.h"
#include "projectmenuimportitemcontroller.h"
#include "projectwatcher.h"
#include "tools.h"
#include "toolsmenuimportitemcontroller.h"
#include "tr.h"
#include "typeslocator.h"

#ifdef WITH_TESTS
#include "libraries/tests/filemodel_tests.h"
#include "libraries/tests/generalmetadataparser_tests.h"
#include "libraries/tests/metadatacheckstatehandler_tests.h"
#include "libraries/tests/metadatamodel_tests.h"
#include "libraries/tests/modulemetadataparser_tests.h"
#include "tests/astxmlparser_tests.h"
#include "tests/autocompleter_tests.h"
#include "tests/documentprocessor_tests.h"
#include "tests/errormessageparser_tests.h"
#include "tests/indenter_tests.h"
#include "tests/linkcreator_tests.h"
#include "tests/modelvalidityguard_tests.h"
#include "tests/parseddatastorage_tests.h"
#include "tests/parseddocumentbuilder_tests.h"
#include "tests/projectcontenthandler_tests.h"
#include "tests/selectionpositionresolver_test.h"
#include "tree-views/tests/combomodel_tests.h"
#include "tree-views/tests/displayrolevisitor_tests.h"
#include "tree-views/tests/outlineindexupdater_tests.h"
#include "tree-views/tests/outlinemodel_tests.h"
#include "tree-views/tests/typestreemodel_tests.h"
#endif

namespace Asn1Acn {
namespace Internal {

class Asn1AcnPluginPrivate
{
public:
    Asn1AcnPluginPrivate(Settings::ServicePtr serviceSettings,
                         Settings::LibrariesPtr librariesSettings,
                         Settings::FuzzerPtr fuzzerSettings)
        : m_optionsPagesService(serviceSettings)
        , m_optionsPagesLibraries(librariesSettings)
        , m_optionsPagesFuzzer(fuzzerSettings)
        , m_componentDirectoryWatcher(librariesSettings)
        , m_asn1sccServiceProvider(serviceSettings)
        , m_kitInformation(new KitInformation)
    {
        ExtensionSystem::PluginManager::addObject(&m_asn1sccServiceProvider);
        ProjectExplorer::KitManager::registerKitInformation(m_kitInformation);

        m_asn1sccServiceProvider.start();
    }

    ~Asn1AcnPluginPrivate()
    {
        ExtensionSystem::PluginManager::removeObject(&m_asn1sccServiceProvider);
        ProjectExplorer::KitManager::deregisterKitInformation(m_kitInformation);
    }

    TreeViews::OutlineWidgetFactory m_outlineWidgetFactory;
    TreeViews::TypesTreeWidgetFactory m_typesTreeWidgetFactory;
    AsnEditorFactory m_asnEditorFactory;
    AcnEditorFactory m_acnEditorFactory;
    ProjectWatcher m_projectWatcher;
    TypesLocator m_typesLocator;

    OptionsPages::Service m_optionsPagesService;
    OptionsPages::Libraries m_optionsPagesLibraries;
    OptionsPages::Fuzzer m_optionsPagesFuzzer;

    Libraries::ComponentDirectoryWatcher m_componentDirectoryWatcher;

    Asn1SccServiceProvider m_asn1sccServiceProvider;

    KitInformation *m_kitInformation;
};

Asn1AcnPlugin::Asn1AcnPlugin() {}

Asn1AcnPlugin::~Asn1AcnPlugin()
{
    delete d;
    d = nullptr;
}

bool Asn1AcnPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    const auto serviceSettings = Settings::load<Settings::Service>();
    const auto librariesSettings = Settings::load<Settings::Libraries>();
    const auto fuzzerSettings = Settings::load<Settings::Fuzzer>();

    d = new Asn1AcnPluginPrivate(serviceSettings, librariesSettings, fuzzerSettings);

    Completion::AsnSnippets::registerGroup();
    Completion::AcnSnippets::registerGroup();

    initializeMenus(fuzzerSettings);

    Core::JsExpander::registerQObjectForJs(QLatin1String("Asn1Acn"), new Asn1AcnJsExtension);

    connect(Core::ProgressManager::instance(),
            &Core::ProgressManager::taskStarted,
            this,
            &Asn1AcnPlugin::onTaskStarted);
    connect(Core::ProgressManager::instance(),
            &Core::ProgressManager::allTasksFinished,
            this,
            &Asn1AcnPlugin::onTaskFinished);

    return true;
}

void Asn1AcnPlugin::initializeMenus(Settings::FuzzerConstPtr fuzzerSettings)
{
    auto toolsMenu = Core::ActionManager::createMenu(Constants::M_TOOLS_ASN);
    auto contextMenu = Core::ActionManager::createMenu(Constants::CONTEXT_MENU);

    Context context(Constants::BASE_CONTEXT);

    initializeSwitchAction(toolsMenu, contextMenu, context);
    initializeOpenInNextSplitAction(toolsMenu, context);
    initializeFollowSymbolAction(toolsMenu, contextMenu);
    initializeFindUsagesAction(toolsMenu, contextMenu, context);
    initializeBuildICDAction(toolsMenu);
    initializeGenerateTestsAction(toolsMenu, fuzzerSettings);
    initializeImportFromAsnComponents(toolsMenu);

    addToToolsMenu(toolsMenu);
}

void Asn1AcnPlugin::initializeFindUsagesAction(ActionContainer *toolsMenu,
                                               ActionContainer *contextMenu,
                                               const Context &context)
{
    auto action = new QAction(tr("Find Usages"), this);
    auto command = Core::ActionManager::registerAction(action, Constants::FIND_USAGES, context);
    command->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+U")));
    connect(action, &QAction::triggered, this, &Asn1AcnPlugin::findUsages);
    contextMenu->addAction(command);
    toolsMenu->addAction(command);
    m_findUsagesAction = action;
}

void Asn1AcnPlugin::addBuildICDToToolsMenu(ActionContainer *toolsMenu)
{
    auto action = new Utils::ParameterAction(tr("Build ICD..."),
                                             tr("Build ICD from \"%1\"..."),
                                             Utils::ParameterAction::AlwaysEnabled,
                                             this);

    action->setEnabled(false);

    connect(ProjectExplorer::SessionManager::instance(),
            &ProjectExplorer::SessionManager::startupProjectChanged,
            [action](ProjectExplorer::Project *project) {
                action->setEnabled(project != nullptr);
                action->setParameter(project == nullptr ? QString() : project->displayName());
            });

    connect(action, &QAction::triggered, [this]() {
        if (m_builder == nullptr)
            m_builder = std::make_shared<ICDBuilder>();
        m_builder->run();
    });

    auto command = Core::ActionManager::registerAction(action, Constants::BUILD_ICD_TOOLBAR);
    command->setAttribute(Core::Command::CA_UpdateText);
    toolsMenu->addAction(command);
}

void Asn1AcnPlugin::addBuildICDToProjectMenu()
{
    QAction *action = new QAction(tr("Build ICD..."), this);

    connect(action, &QAction::triggered, [this]() {
        if (m_builder == nullptr)
            m_builder = std::make_shared<ICDBuilder>();
        m_builder->run(ProjectExplorer::ProjectTree::currentProject());
    });

    Core::Context projectTreeContext(ProjectExplorer::Constants::C_PROJECT_TREE);
    auto menu = Core::ActionManager::createMenu(ProjectExplorer::Constants::M_PROJECTCONTEXT);
    auto command = Core::ActionManager::registerAction(action,
                                                       Constants::BUILD_ICD_CONTEXT,
                                                       projectTreeContext);
    menu->addAction(command, ProjectExplorer::Constants::G_PROJECT_FILES);
}

void Asn1AcnPlugin::initializeGenerateTestsAction(ActionContainer *toolsMenu,
                                                  Settings::FuzzerConstPtr settings)
{
    auto action = new Utils::ParameterAction(tr("Execute asn1scc.Fuzzer..."),
                                             tr("Execute asn1scc.Fuzzer \"%1\"..."),
                                             Utils::ParameterAction::AlwaysEnabled,
                                             this);
    action->setEnabled(false);

    connect(ProjectExplorer::SessionManager::instance(),
            &ProjectExplorer::SessionManager::startupProjectChanged,
            [action](ProjectExplorer::Project *project) {
                action->setEnabled(project != nullptr);
                action->setParameter(project == nullptr ? QString() : project->displayName());
            });

    connect(action, &QAction::triggered, [this, settings]() {
        if (m_fuzzerDialog.isNull()) {
            auto paramsProvider = std::make_shared<Fuzzer::FuzzerParamsProvider>(settings);
            m_fuzzerDialog = new Fuzzer::FuzzerParamsDialog(paramsProvider);
        }
        m_fuzzerDialog->exec();
    });

    auto command = Core::ActionManager::registerAction(action, Constants::GENERATE_TESTS);
    command->setAttribute(Core::Command::CA_UpdateText);
    toolsMenu->addAction(command);
}

void Asn1AcnPlugin::initializeBuildICDAction(ActionContainer *toolsMenu)
{
    addBuildICDToProjectMenu();
    addBuildICDToToolsMenu(toolsMenu);
}

static EditorWidget *currentEditorWidget()
{
    if (auto currentEditor = Core::EditorManager::currentEditor())
        return qobject_cast<EditorWidget *>(currentEditor->widget());
    return nullptr;
}

void Asn1AcnPlugin::findUsages()
{
    if (auto editorWidget = currentEditorWidget())
        editorWidget->findUsages();
}

void Asn1AcnPlugin::initializeSwitchAction(ActionContainer *toolsMenu,
                                           ActionContainer *contextMenu,
                                           const Context &context)
{
    QAction *switchAction = new QAction(tr("Switch Data/Encoding"), this);
    Core::Command *command = Core::ActionManager::registerAction(switchAction,
                                                                 Constants::SWITCH_DATA_ENCODING,
                                                                 context,
                                                                 true);
    command->setDefaultKeySequence(QKeySequence(Qt::Key_F4));
    connect(switchAction, &QAction::triggered, []() { Tools::switchBetweenDataAndEncoding(); });
    toolsMenu->addAction(command);
    contextMenu->addAction(command);
}

void Asn1AcnPlugin::initializeFollowSymbolAction(ActionContainer *toolsMenu,
                                                 ActionContainer *contextMenu)
{
    Core::Command *command = Core::ActionManager::ActionManager::command(
        TextEditor::Constants::FOLLOW_SYMBOL_UNDER_CURSOR);
    contextMenu->addAction(command);
    toolsMenu->addAction(command);
}

void Asn1AcnPlugin::initializeOpenInNextSplitAction(ActionContainer *toolsMenu,
                                                    const Context &context)
{
    QAction *openInNextSplitAction
        = new QAction(tr("Open Corresponding Data/Encoding in Next Split"), this);
    Core::Command *command
        = Core::ActionManager::registerAction(openInNextSplitAction,
                                              Constants::OPEN_DATA_ENCODING_IN_NEXT_SPLIT,
                                              context,
                                              true);
    command->setDefaultKeySequence(
        QKeySequence(Utils::HostOsInfo::isMacHost() ? tr("Meta+E, F4") : tr("Ctrl+E, F4")));
    connect(openInNextSplitAction, &QAction::triggered, []() {
        Tools::switchBetweenDataAndEncodingInNextSplit();
    });
    toolsMenu->addAction(command);
}

void Asn1AcnPlugin::addImportFromAsnComponentsToToolsMenu(ActionContainer *toolsMenu)
{
    toolsMenu->addSeparator();

    auto action = new Utils::ParameterAction(tr("Import from ASN.1 components library..."),
                                             tr("Import from ASN.1 components library to \"%1\"..."),
                                             Utils::ParameterAction::AlwaysEnabled,
                                             this);

    connect(action, &QAction::triggered, [this]() {
        raiseImportComponentWindow(ProjectExplorer::SessionManager::startupProject());
    });

    action->setEnabled(false);

    new ToolsMenuImportItemController(action, this);

    auto command
        = Core::ActionManager::registerAction(action,
                                              Constants::IMPORT_FROM_COMPONENTS_LIBRARY_TOOLBAR);
    command->setAttribute(Core::Command::CA_UpdateText);
    toolsMenu->addAction(command);
}

void Asn1AcnPlugin::addImportFromAsnComponentsToProjectMenu()
{
    QAction *action = new QAction(tr("Import from ASN.1 components library..."), this);

    connect(action, &QAction::triggered, [this]() {
        raiseImportComponentWindow(ProjectExplorer::ProjectTree::currentProject());
    });

    action->setEnabled(false);

    new ProjectMenuImportItemController(action, this);

    Core::Context projectTreeContext(ProjectExplorer::Constants::C_PROJECT_TREE);
    auto menu = Core::ActionManager::createMenu(ProjectExplorer::Constants::M_PROJECTCONTEXT);
    auto command
        = Core::ActionManager::registerAction(action,
                                              Constants::IMPORT_FROM_COMPONENTS_LIBRARY_CONTEXT,
                                              projectTreeContext);
    menu->addAction(command, ProjectExplorer::Constants::G_PROJECT_FILES);
}

void Asn1AcnPlugin::initializeImportFromAsnComponents(ActionContainer *toolsMenu)
{
    addImportFromAsnComponentsToToolsMenu(toolsMenu);
    addImportFromAsnComponentsToProjectMenu();
}

void Asn1AcnPlugin::addToToolsMenu(ActionContainer *container)
{
    auto toolsMenu = Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    QMenu *menu = container->menu();
    menu->setTitle(Tr::tr("&ASN.1/ACN"));
    menu->setEnabled(true);
    toolsMenu->addMenu(container);
}

void Asn1AcnPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag Asn1AcnPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void Asn1AcnPlugin::raiseImportComponentWindow(ProjectExplorer::Project *project)
{
    if (!m_importComponentWizard.isNull()) {
        m_importComponentWizard->setProject(project);
        Core::ICore::raiseWindow(m_importComponentWizard);
    } else {
        m_importComponentWizard
            = new Libraries::Wizard::ImportComponentWizard(project, Core::ICore::mainWindow());
        m_importComponentWizard->show();
    }
}

void Asn1AcnPlugin::onTaskStarted(Core::Id id)
{
    if (id == Constants::TASK_SEARCH) {
        m_findUsagesAction->setEnabled(false);
    }
}

void Asn1AcnPlugin::onTaskFinished(Core::Id id)
{
    if (id == Constants::TASK_SEARCH) {
        m_findUsagesAction->setEnabled(true);
    }
}

#ifdef WITH_TESTS
QList<QObject *> Asn1AcnPlugin::createTestObjects() const
{
    return QList<QObject *>() << new Libraries::Tests::ModuleMetadataParserTests
                              << new Libraries::Tests::GeneralMetadataParserTests
                              << new Libraries::Tests::MetadataModelTests
                              << new Libraries::Tests::MetadataCheckStateHandlerTests
                              << new Libraries::Tests::FileModelTests
                              << new TreeViews::Tests::DisplayRoleVisitorTests
                              << new TreeViews::Tests::OutlineIndexUpdaterTests
                              << new TreeViews::Tests::OutlineModelTests
                              << new TreeViews::Tests::ComboModelTests
                              << new TreeViews::Tests::TypesTreeModelTests
                              << new Tests::AstXmlParserTests << new Tests::ErrorMessageParserTests
                              << new Tests::ParsedDocumentBuilderTests
                              << new Tests::DocumentProcessorTests
                              << new Tests::ProjectContentHandlerTests
                              << new Tests::ParsedDataStorageTests << new Tests::AutoCompleterTests
                              << new Tests::LinkCreatorTests << new Tests::IndenterTests
                              << new Tests::SelectionPositionResolverTests;
}
#endif

} // namespace Internal
} // namespace Asn1Acn

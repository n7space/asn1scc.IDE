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

#include "completion/acnsnippets.h"
#include "completion/asnsnippets.h"

#include "settings/libraries.h"
#include "settings/service.h"
#include "settings/settings.h"

#include "options-pages/libraries.h"
#include "options-pages/service.h"

#include "tree-views/outlinewidget.h"
#include "tree-views/typestreewidget.h"

#include "libraries/componentdirectorywatcher.h"
#include "libraries/componentlibrarydispatcher.h"

#include "acneditor.h"
#include "asn1acnconstants.h"
#include "asn1acnjsextension.h"
#include "asn1sccserviceprovider.h"
#include "asneditor.h"
#include "importmenuitemcontroller.h"
#include "kitinformation.h"
#include "projectwatcher.h"
#include "tools.h"
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
#include "tests/sourcemapper_tests.h"
#include "tree-views/tests/combomodel_tests.h"
#include "tree-views/tests/displayrolevisitor_tests.h"
#include "tree-views/tests/outlineindexupdater_tests.h"
#include "tree-views/tests/outlinemodel_tests.h"
#include "tree-views/tests/typestreemodel_tests.h"
#endif

namespace Asn1Acn {
namespace Internal {

Asn1AcnPlugin::Asn1AcnPlugin()
{
    // Create your members
}

Asn1AcnPlugin::~Asn1AcnPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool Asn1AcnPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    const auto serviceSettings = Settings::load<Settings::Service>();
    const auto librariesSettings = Settings::load<Settings::Libraries>();

    addAutoReleasedObject(new AsnEditorFactory);
    addAutoReleasedObject(new AcnEditorFactory);

    addAutoReleasedObject(new ProjectWatcher);

    addAutoReleasedObject(new TreeViews::OutlineWidgetFactory);
    addAutoReleasedObject(new TreeViews::TypesTreeWidgetFactory);

    addAutoReleasedObject(new OptionsPages::Service(serviceSettings));
    addAutoReleasedObject(new OptionsPages::Libraries(librariesSettings));

    addAutoReleasedObject(new TypesLocator);

    const auto directoryWatcher = new Libraries::ComponentDirectoryWatcher(librariesSettings);
    addAutoReleasedObject(directoryWatcher);

    Completion::AsnSnippets::registerGroup();
    Completion::AcnSnippets::registerGroup();

    Asn1SccServiceProvider *sp = new Asn1SccServiceProvider(serviceSettings);
    addAutoReleasedObject(sp);
    sp->start();

    initializeMenus();

    Core::JsExpander::registerQObjectForJs(QLatin1String("Asn1Acn"), new Asn1AcnJsExtension);

    connect(Core::ProgressManager::instance(),
            &Core::ProgressManager::taskStarted,
            this,
            &Asn1AcnPlugin::onTaskStarted);
    connect(Core::ProgressManager::instance(),
            &Core::ProgressManager::allTasksFinished,
            this,
            &Asn1AcnPlugin::onTaskFinished);

    ProjectExplorer::KitManager::registerKitInformation(new KitInformation);

    return true;
}

void Asn1AcnPlugin::initializeMenus()
{
    auto toolsMenu = Core::ActionManager::createMenu(Constants::M_TOOLS_ASN);
    auto contextMenu = Core::ActionManager::createMenu(Constants::CONTEXT_MENU);

    Context context(Constants::BASE_CONTEXT);

    initializeSwitchAction(toolsMenu, contextMenu, context);
    initializeOpenInNextSplitAction(toolsMenu, context);
    initializeFollowSymbolAction(toolsMenu, contextMenu);
    initializeFindUsagesAction(toolsMenu, contextMenu, context);
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

void Asn1AcnPlugin::initializeImportFromAsnComponents(ActionContainer *toolsMenu)
{
    toolsMenu->addSeparator();

    QAction *importFromAsnComponents = new QAction(tr("Import from ASN.1 components library..."),
                                                   this);
    Core::Command *command
        = Core::ActionManager::registerAction(importFromAsnComponents,
                                              Constants::IMPORT_FROM_COMPONENTS_LIBRARY);
    connect(importFromAsnComponents,
            &QAction::triggered,
            this,
            &Asn1AcnPlugin::raiseImportComponentWindow);
    toolsMenu->addAction(command);
    importFromAsnComponents->setEnabled(false);

    new ImportMenuItemController(importFromAsnComponents, this);
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

void Asn1AcnPlugin::raiseImportComponentWindow()
{
    if (!m_importComponentWizard.isNull()) {
        Core::ICore::raiseWindow(m_importComponentWizard);
    } else {
        m_importComponentWizard = new Libraries::Wizard::ImportComponentWizard(
            Core::ICore::mainWindow());
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
                              << new Tests::SourceMapperTests << new Tests::ModelValidityGuardTests
                              << new Tests::LinkCreatorTests << new Tests::IndenterTests;
}
#endif

} // namespace Internal
} // namespace Asn1Acn

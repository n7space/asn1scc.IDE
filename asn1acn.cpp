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

#include <QMenu>

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/jsexpander.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <extensionsystem/pluginmanager.h>

#include <texteditor/texteditorconstants.h>

#include "completion/asnsnippetprovider.h"
#include "asneditor.h"
#include "outline.h"
#include "structuresview.h"
#include "projectwatcher.h"
#include "asn1acnjsextension.h"
#include "asn1sccserviceprovider.h"

#include "settings/settings.h"
#include "settings/general.h"
#include "settings/service.h"

#include "options-pages/general.h"
#include "options-pages/service.h"

#include "acneditor.h"
#include "asn1acnconstants.h"
#include "tr.h"
#include "tools.h"

#ifdef WITH_TESTS
#include "tests/astxmlparser_tests.h"
#endif

#include "asn1acn.h"

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

    const auto generalSettings = Settings::load<Settings::General>();
    const auto serviceSettings = Settings::load<Settings::Service>();

    addAutoReleasedObject(new AsnEditorFactory);
    addAutoReleasedObject(new StructuresViewFactory);
    addAutoReleasedObject(new Completion::AsnSnippetProvider);

    addAutoReleasedObject(new AcnEditorFactory);

    addAutoReleasedObject(new OutlineWidgetFactory);

    addAutoReleasedObject(new ProjectWatcher);

    addAutoReleasedObject(new OptionsPages::General(generalSettings));
    addAutoReleasedObject(new OptionsPages::Service(serviceSettings));

    Asn1SccServiceProvider *sp = new Asn1SccServiceProvider(serviceSettings);
    addAutoReleasedObject(sp);
    sp->start();

    initializeMenus();

    Core::JsExpander::registerQObjectForJs(QLatin1String("Asn1Acn"), new Asn1AcnJsExtension);

    return true;
}

static void addToToolsMenu(Core::ActionContainer* container)
{
    auto toolsMenu = Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    QMenu *menu = container->menu();
    menu->setTitle(Tr::tr("&ASN.1/ACN"));
    menu->setEnabled(true);
    toolsMenu->addMenu(container);
}

void Asn1AcnPlugin::initializeMenus()
{
    auto asnToolsMenu = Core::ActionManager::createMenu(Constants::M_TOOLS_ASN);
    auto contextMenu = Core::ActionManager::createMenu(Constants::CONTEXT_MENU);

    Core::Context context(Constants::BASE_CONTEXT);

    QAction *switchAction = new QAction(tr("Switch Data/Encoding"), this);
    Core::Command *command = Core::ActionManager::registerAction(switchAction, Constants::SWITCH_DATA_ENCODING, context, true);
    command->setDefaultKeySequence(QKeySequence(Qt::Key_F4));
    connect(switchAction, &QAction::triggered, []() { Tools::switchBetweenDataAndEncoding(); });
    asnToolsMenu->addAction(command);
    contextMenu->addAction(command);

    QAction *openInNextSplitAction = new QAction(tr("Open Corresponding Data/Encoding in Next Split"), this);
    command = Core::ActionManager::registerAction(openInNextSplitAction, Constants::OPEN_DATA_ENCODING_IN_NEXT_SPLIT, context, true);
    command->setDefaultKeySequence(QKeySequence(Utils::HostOsInfo::isMacHost() ? tr("Meta+E, F4") : tr("Ctrl+E, F4")));
    connect(openInNextSplitAction, &QAction::triggered, []() { Tools::switchBetweenDataAndEncodingInNextSplit(); });
    asnToolsMenu->addAction(command);

    command = Core::ActionManager::ActionManager::command(TextEditor::Constants::FOLLOW_SYMBOL_UNDER_CURSOR);
    contextMenu->addAction(command);
    asnToolsMenu->addAction(command);

    addToToolsMenu(asnToolsMenu);
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

#ifdef WITH_TESTS
QList<QObject *> Asn1AcnPlugin::createTestObjects() const
{
    return QList<QObject *>()
            << new Tests::AstXmlParserTests
               ;
}
#endif

} // namespace Internal
} // namespace Asn1Acn

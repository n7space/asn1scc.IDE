#include "asn1sccplugin.h"
#include "asn1sccpluginconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

namespace asn1sccplugin {
namespace Internal {

asn1sccPlugin::asn1sccPlugin()
{
    // Create your members
}

asn1sccPlugin::~asn1sccPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool asn1sccPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    QAction *action = new QAction(tr("asn1sccplugin Action"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, &QAction::triggered, this, &asn1sccPlugin::triggerAction);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("asn1sccplugin"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    return true;
}

void asn1sccPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag asn1sccPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void asn1sccPlugin::triggerAction()
{
    QMessageBox::information(Core::ICore::mainWindow(),
                             tr("Action Triggered"),
                             tr("This is an action from asn1sccplugin."));
}

} // namespace Internal
} // namespace asn1sccplugin

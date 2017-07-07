/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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
#include "asn1acnconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/jsexpander.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <extensionsystem/pluginmanager.h>

#include <texteditor/texteditorconstants.h>

#include "asneditor.h"
#include "outline.h"
#include "asnsnippetprovider.h"
#include "structuresview.h"
#include "projectwatcher.h"
#include "asn1acnjsextension.h"
#include "asn1sccserviceprovider.h"

#include "acneditor.h"

#ifdef WITH_TESTS
#include "tests/astxmlparser_tests.h"
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

    setDefaultSettings();

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    addAutoReleasedObject(new AsnEditorFactory);
    addAutoReleasedObject(new StructuresViewFactory);
    addAutoReleasedObject(new AsnSnippetProvider);

    addAutoReleasedObject(new AcnEditorFactory);

    addAutoReleasedObject(new OutlineWidgetFactory);

    addAutoReleasedObject(new ProjectWatcher);

    Asn1SccServiceProvider *sp = new Asn1SccServiceProvider;
    addAutoReleasedObject(sp);
    sp->start();

    Core::ActionContainer *contextMenu = Core::ActionManager::createMenu(Constants::CONTEXT_MENU);

    Core::Command *cmd = Core::ActionManager::ActionManager::command(TextEditor::Constants::FOLLOW_SYMBOL_UNDER_CURSOR);
    contextMenu->addAction(cmd);

    Core::JsExpander::registerQObjectForJs(QLatin1String("Asn1Acn"), new Asn1AcnJsExtension);

    return true;
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

void Asn1AcnPlugin::setDefaultSettings()
{
    // TODO: loading of settings needs to be more sophisticated.
    // Now on Linux OS settings can be found in ~/.config/QtProject/QtCreator.ini

    QSettings *s = Core::ICore::settings();

    s->beginGroup(Constants::ASN1ACN_GROUP_NAME);

    if (!s->contains(Constants::ASN1ACN_SERVICE_PATH))
        s->setValue(Constants::ASN1ACN_SERVICE_PATH,
                    "/opt/asn1sccDaemon/asn1scc/Daemon/bin/Debug/Daemon.exe");

    if (!s->contains(Constants::ASN1ACN_SERVICE_PORT))
        s->setValue(Constants::ASN1ACN_SERVICE_PORT, 9749);

    if (!s->contains(Constants::ASN1ACN_SERVICE_WATCHDOG))
        s->setValue(Constants::ASN1ACN_SERVICE_WATCHDOG, 1000);

    s->endGroup();
    s->sync();
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

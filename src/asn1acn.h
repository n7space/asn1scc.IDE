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

#include "asn1acn_global.h"

#include <extensionsystem/iplugin.h>

#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actioncontainer.h>

#include "libraries/component-importer/importcomponentdialog.h"

namespace Asn1Acn {
namespace Internal {

class Asn1AcnPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Asn1Acn.json")

public:
    Asn1AcnPlugin();
    ~Asn1AcnPlugin();

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private slots:
    void raiseImportComponentWindow();

private:
    using ActionContainer = Core::ActionContainer;
    using Context = Core::Context;

    void initializeMenus();

    void initializeSwitchActionMenu(ActionContainer *toolsMenu, ActionContainer *contextMenu, const Context &context);
    void initializeFollowSymbolActionMenu(ActionContainer *toolsMenu, ActionContainer *contextMenu);
    void initializeOpenInNextSplitActionMenu(ActionContainer *toolsMenu, const Context &context);
    void initializeImportFromAsnComponents(ActionContainer *toolsMenu);

    void addToToolsMenu(ActionContainer *container);

    QPointer<ComponentImporter::ImportComponentDialog> m_importComponentDialog;

#ifdef WITH_TESTS   
    QList<QObject *> createTestObjects() const override;
#endif
};

} // namespace Internal
} // namespace Asn1Acn

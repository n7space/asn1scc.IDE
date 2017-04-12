#pragma once

#include "asn1sccplugin_global.h"

#include <extensionsystem/iplugin.h>

namespace asn1sccplugin {
namespace Internal {

class asn1sccPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "asn1sccplugin.json")

public:
    asn1sccPlugin();
    ~asn1sccPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void triggerAction();
};

} // namespace Internal
} // namespace asn1sccplugin

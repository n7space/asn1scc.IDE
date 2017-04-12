#pragma once

#include "asn1acn_global.h"

#include <extensionsystem/iplugin.h>

namespace asn1acn {
namespace Internal {

class asn1acnPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "asn1acn.json")

public:
    asn1acnPlugin();
    ~asn1acnPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void triggerAction();
};

} // namespace Internal
} // namespace asn1acn

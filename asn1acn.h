#pragma once

#include "asn1acn_global.h"

#include <extensionsystem/iplugin.h>

namespace Asn1Acn {
namespace Internal {

class Asn1AcnPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "asn1acn.json")

public:
    Asn1AcnPlugin();
    ~Asn1AcnPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void triggerAction();
};

} // namespace Internal
} // namespace Asn1Acn

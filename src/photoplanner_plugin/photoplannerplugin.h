#ifndef PHOTOPLANNERPLUGIN_H
#define PHOTOPLANNERPLUGIN_H

#include "App/PluginInterface.h"

class PhotoplannerPlugin: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.uavos.gcs.PluginInterface/1.0")
    Q_INTERFACES(PluginInterface)
public:
    PhotoplannerPlugin();
    int flags() override;
    QObject *createControl() override;
    QStringList depends() override;
};

#endif // PHOTOPLANNERPLUGIN_H

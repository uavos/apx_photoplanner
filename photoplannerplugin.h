#ifndef PHOTOPLANNERPLUGIN_H
#define PHOTOPLANNERPLUGIN_H

#include "ApxPluginInterface.h"

class PhotoplannerPlugin: public ApxPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.uavos.gcs.ApxPluginInterface/1.0")
    Q_INTERFACES(ApxPluginInterface)
public:
    PhotoplannerPlugin();
    QObject *createControl();
};

#endif // PHOTOPLANNERPLUGIN_H

#include "photoplannerplugin.h"

#include "photoplanner.h"

PhotoplannerPlugin::PhotoplannerPlugin()
{
}

QObject *PhotoplannerPlugin::createControl()
{
    return new Photoplanner();
}

#include "photoplannerplugin.h"

#include "photoplanner.h"

PhotoplannerPlugin::PhotoplannerPlugin()
{
}

int PhotoplannerPlugin::flags()
{
    return Feature | Map;
}

QObject *PhotoplannerPlugin::createControl()
{
    return new ApxPhotoplanner();
}

QStringList PhotoplannerPlugin::depends() { return QStringList() << "MissionPlanner"; }

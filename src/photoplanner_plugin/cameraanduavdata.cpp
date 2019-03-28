#include "cameraanduavdata.h"

#include <QStringList>
#include "ApxLog.h"

QString UavData::toString()
{
    QString str = QString("%1/%2/%3/%4/%5")
            .arg(name)
            .arg(flightTime)
            .arg(flightSpeed)
            .arg(maxRoll)
            .arg(commRadius);
    return str;
}

void UavData::fromString(const QString &data)
{
    QStringList tokens = data.split('/');
    if(tokens.size() == 5)
    {
        name = tokens[0];
        flightTime = tokens[1].toInt();
        flightSpeed = tokens[2].toInt();
        maxRoll = tokens[3].toInt();
        commRadius = tokens[4].toInt();
    }
    else
        apxDebug() << "can't parse uav data: " << data;
}

QString CameraData::toString()
{
    QString str = QString("%1/%2/%3/%4/%5/%6")
            .arg(name)
            .arg(focus)
            .arg(sensorLx)
            .arg(sensorLy)
            .arg(sensorAx)
            .arg(sensorAy);
    return str;
}

void CameraData::fromString(const QString &data)
{
    QStringList tokens = data.split('/');
    if(tokens.size() == 6)
    {
        name = tokens[0];
        focus = tokens[1].toInt();
        sensorLx = tokens[2].toFloat();
        sensorLy = tokens[3].toFloat();
        sensorAx = tokens[4].toInt();
        sensorAy = tokens[5].toInt();
    }
    else
        apxDebug() << "can't parse camera data: " << data;
}

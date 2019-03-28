#ifndef CAMERAANDUAVDATA_H
#define CAMERAANDUAVDATA_H

#include <QString>

struct UavData
{
    QString name;
    int flightTime;
    int flightSpeed;
    int maxRoll;
    int commRadius;
    QString toString();
    void fromString(const QString &data);
};

struct CameraData
{
    QString name;
    int focus;
    float sensorLx;
    float sensorLy;
    int sensorAx;
    int sensorAy;
    QString toString();
    void fromString(const QString &data);
};

#endif // CAMERAANDUAVDATA_H

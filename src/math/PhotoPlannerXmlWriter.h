#pragma once

#include "PhotoPlannerCore.h"

#include <QDateTime>
#include <QXmlStreamWriter>
#include <QFile>

namespace aero_photo
{

class PhotoPlannerXmlWriter
{
public:
    PhotoPlannerXmlWriter(const FlightPoints &flightPoints, qreal velocity);
    PhotoPlannerXmlWriter() = delete;
    PhotoPlannerXmlWriter(const PhotoPlannerXmlWriter &) = delete;
    PhotoPlannerXmlWriter(PhotoPlannerXmlWriter &&) = delete;

    void WritePackets(QString fileurlcvt, const int totalPointsInPacket);
    void WritePacket(QXmlStreamWriter &stream, int packetNumber, int totalPointsInPacket);
    void WriteHome(QXmlStreamWriter &stream, const GeoPoint &homePoint);
    void WriteRunway(QXmlStreamWriter &stream, const GeoPoint &runwayPoint);
    void WritePoints(QXmlStreamWriter &stream, int packetNumber, int totalPointsInPacket);
    void WriteOnePoint(QXmlStreamWriter &stream, int index, const FlightPoint &flightPoint);
    void WriteOnePointActions(QXmlStreamWriter &stream, const FlightPoint &flightPoint);
    const FlightPoints &GetFlightPoints() const;
    qreal velocity() const;

    const FlightPoints &flightPoints_;
    const qreal velocity_;
    bool isSpeedWritten_ = false;
};

} // namespace aero_photo

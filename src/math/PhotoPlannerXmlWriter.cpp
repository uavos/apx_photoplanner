#include "PhotoPlannerXmlWriter.h"

namespace aero_photo
{

PhotoPlannerXmlWriter::PhotoPlannerXmlWriter(const FlightPoints &flightPoints, qreal velocity):
    flightPoints_(flightPoints), velocity_(velocity)
{
}

void PhotoPlannerXmlWriter::WritePackets(QString fileurlcvt, const int totalPointsInPacket)
{
    fileurlcvt = fileurlcvt.remove(".xml");
    for(int i = 0; totalPointsInPacket * i < GetFlightPoints().size(); i++) {
        auto packname = QString("%1-%2.xml").arg(fileurlcvt).arg(i);
        if(i == 0) {
            packname = QString("%1.xml").arg(fileurlcvt);
        }
        QFile xmlFile(packname);
        if(xmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QXmlStreamWriter stream(&xmlFile);
            stream.setAutoFormatting(true);
            stream.writeStartDocument();
            WritePacket(stream, i, totalPointsInPacket);
            stream.writeEndDocument();
        }
    }
}

void PhotoPlannerXmlWriter::WritePacket(QXmlStreamWriter &stream, int packetNumber, int totalPointsInPacket)
{
    isSpeedWritten_ = false;

    stream.writeStartElement("mission");
    stream.writeAttribute("version", "9.10.56");
    stream.writeAttribute("href", "http://www.uavos.com/");
    stream.writeAttribute("title", "mission");
    stream.writeTextElement("hash", "f7c8603a8b1af004375ebc326a0ab6e");
    stream.writeTextElement("timestamp", QDateTime::currentDateTime().toString());
    if(packetNumber == 0 && !GetFlightPoints().empty())
        WriteHome(stream, GetFlightPoints().front());
    if(!GetFlightPoints().empty())
        WriteRunway(stream, GetFlightPoints().front());
    WritePoints(stream, packetNumber, totalPointsInPacket);
    stream.writeEndElement();
}
void PhotoPlannerXmlWriter::WriteHome(QXmlStreamWriter &stream, const GeoPoint &homePoint)
{
    stream.writeStartElement("home");
    stream.writeTextElement("lat", QString::number(homePoint.latitude(), 'f', 15));
    stream.writeTextElement("lon", QString::number(homePoint.longitude(), 'f', 15));
    stream.writeTextElement("hmsl", "0");
    stream.writeEndElement();
}
void PhotoPlannerXmlWriter::WriteRunway(QXmlStreamWriter &stream, const GeoPoint &runwayPoint)
{
    stream.writeStartElement("runways");
    stream.writeAttribute("cnt", "1");
    stream.writeStartElement("runway");
    stream.writeAttribute("idx", "0");
    stream.writeTextElement("turn", "0");
    stream.writeTextElement("approach", "400");
    stream.writeTextElement("HMSL", "0");
    stream.writeTextElement("latitude", QString::number(runwayPoint.latitude(), 'f', 15));
    stream.writeTextElement("longitude", QString::number(runwayPoint.longitude(), 'f', 15));
    stream.writeTextElement("dN", "150");
    stream.writeTextElement("dE", "0");
    stream.writeEndElement();
    stream.writeEndElement();
}
void PhotoPlannerXmlWriter::WritePoints(QXmlStreamWriter &stream, int packetNumber, int totalPointsInPacket)
{
    stream.writeStartElement("waypoints");
    const int packetOffset = packetNumber * totalPointsInPacket;
    totalPointsInPacket = std::min(totalPointsInPacket, GetFlightPoints().size() - packetOffset);
    stream.writeAttribute("cnt", QString::number(totalPointsInPacket));
    const auto &points = GetFlightPoints();
    for(int index = 0; index < totalPointsInPacket; index++) {
        WriteOnePoint(stream, index, points[packetOffset + index]);
    }
    stream.writeEndElement();
}
void PhotoPlannerXmlWriter::WriteOnePoint(QXmlStreamWriter &stream, int index, const FlightPoint &flightPoint)
{
    // Conversion???
    stream.writeStartElement("waypoint");
    stream.writeAttribute("idx", QString::number(index));
    stream.writeTextElement("altitude", QString::number(flightPoint.altitude()));
    stream.writeTextElement("type", QString::number(flightPoint.type()));
    stream.writeTextElement("latitude", QString::number(flightPoint.latitude(), 'f', 15));
    stream.writeTextElement("longitude", QString::number(flightPoint.longitude(), 'f', 15));
    WriteOnePointActions(stream, flightPoint);
    stream.writeEndElement();
}

void PhotoPlannerXmlWriter::WriteOnePointActions(QXmlStreamWriter &stream, const FlightPoint &flightPoint)
{
    stream.writeStartElement("actions");
    if(!isSpeedWritten_) {
        stream.writeTextElement("speed", QString::number(velocity()));
        isSpeedWritten_ = true;
    }
    stream.writeTextElement("shot", (flightPoint.shotDistance() > 0) ? "2" : "0");
    stream.writeTextElement("dshot", QString::number(flightPoint.shotDistance()));
    stream.writeTextElement("POI", "0");
    stream.writeTextElement("loiter", "0");
    stream.writeTextElement("turnR", "0");
    stream.writeTextElement("loops", "0");
    stream.writeTextElement("time", "0");
    stream.writeEndElement();
}

const FlightPoints &PhotoPlannerXmlWriter::GetFlightPoints() const
{
    return flightPoints_;
}
qreal PhotoPlannerXmlWriter::velocity() const
{
    return velocity_;
}

} // namespace aero_photo

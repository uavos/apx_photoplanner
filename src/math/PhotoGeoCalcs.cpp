#include "PhotoGeoCalcs.h"

namespace aero_photo
{

qreal GeoCalc::Azimuth(const GeoPoint &pntA, const GeoPoint &pntB)
{
    return pntA.azimuthTo(pntB);
}
GeoDistance GeoCalc::Distance(const GeoPoint &pntA, const GeoPoint &pntB)
{
    return pntA.distanceTo(pntB);
}
GeoPoint GeoCalc::AtDistanceAndAzimuth(const GeoPoint &pnt, GeoDistance distance, qreal azimuth)
{
    return pnt.atDistanceAndAzimuth(distance, azimuth);
}

GeoDistance GeoCalc::RoundUpPoints(GeoPoint &pntA, GeoPoint &pntB, GeoDistance factor, bool fixA, bool fixB)
{
    auto distance = Distance(pntA, pntB);
    if(fixA && fixB)
        return distance;

    auto deltaDistance = ceil(distance / factor) * factor - distance;
    if(deltaDistance != 0) {
        auto azimuthAB = Azimuth(pntA, pntB);
        auto azimuthBA = azimuthAB + 180;
        distance += deltaDistance;
        if(!fixA && !fixB) {
            pntA = AtDistanceAndAzimuth(pntA, deltaDistance / 2, azimuthBA);
            pntB = AtDistanceAndAzimuth(pntA, distance, azimuthAB);
        } else if(fixA) {
            pntB = AtDistanceAndAzimuth(pntA, distance, azimuthAB);
        } else { // if (fixB)
            pntA = AtDistanceAndAzimuth(pntB, distance, azimuthBA);
        }
    }
    return distance;
}

RunStartPointsCalc::RunStartPointsCalc(qreal enterAzimuth, const GeoPoint &baseRunPoint, qreal runAzimuth, qreal Lyp, size_t totalRuns):
    basePoint_(baseRunPoint),
    startPointsAzimuth_(enterAzimuth + 90 + (runAzimuth - enterAzimuth) / 2),
    Lyp_(Lyp / cos(qDegreesToRadians((runAzimuth - enterAzimuth) / 2))),
    indexOffset_(totalRuns / 2 - totalRuns + 1)
{
}

GeoPoint RunStartPointsCalc::Calculate(int index, qreal offset)
{
    // Indexes from left to right in azimuth direction
    auto startPoint = basePoint_.atDistanceAndAzimuth(Lyp_ * (indexOffset_ + index) + offset, startPointsAzimuth_);
    return startPoint;
}
CartesianCalcs::CartesianCalcs(GeoPoint basePoint, qreal baseAzimuth):
    basePoint_(basePoint), baseAzimuth_(baseAzimuth)
{
}

QPointF CartesianCalcs::ConvertToCartesian(const GeoPoint &point)
{
    auto azimuthToPoint = qDegreesToRadians(basePoint_.azimuthTo(point));
    auto distanceToPoint = basePoint_.distanceTo(point);
    return QPointF(distanceToPoint * cos(azimuthToPoint), distanceToPoint * sin(azimuthToPoint));
}

} // namespace aero_photo
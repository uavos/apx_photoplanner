#include "ManeuverTrackAlignment.h"

namespace aero_photo
{
ManeuverTrackAlignment::ManeuverTrackAlignment(const GeoPoint &pnt1, qreal az1, const GeoPoint &pnt2, qreal az2):
    pnt1_(pnt1), az1_(az1), pnt2_(pnt2), az2_(az2), distance12_(pnt1.distanceTo(pnt2)), az12_(pnt1.azimuthTo(pnt2))
{
}

GeoPoints ManeuverTrackAlignment::Calculate(const PhotoUavModel &uavModel)
{
    auto R = uavModel.GetManeuverR();

    if(2 * R < distance12_)
        return CalculateMoreThen2RAligment(R);

    //if ( distance12_ < R)
    {
    }

    ManeuverTrackAlignment enlargedEntry(pnt1_, az1_, pnt2_.atDistanceAndAzimuth(R, az2_ + Azimuth(180)), az2_);
    auto geoPoints = enlargedEntry.Calculate(uavModel);
    flightAligment_ = enlargedEntry.GetFlightPoints();
    geoPoints.push_back(pnt2_);
    return geoPoints;
}

GeoPoints ManeuverTrackAlignment::GetFlightPoints() const
{
    return flightAligment_;
}

GeoPoints ManeuverTrackAlignment::CalculateMoreThen2RAligment(qreal R)
{
    auto t1R = GetTurnPointData(R, pnt1_, az1_, true);
    auto t1L = GetTurnPointData(R, pnt1_, az1_, false);
    auto t2R = GetTurnPointData(R, pnt2_, az2_, true);
    auto t2L = GetTurnPointData(R, pnt2_, az2_, false);

    std::map<qreal, AligmentResult> maneuverLengths;
    auto addTurnPairsChecked = [this, &maneuverLengths, R](auto &&t1, auto &&t2) {
        auto distance = t1.center.distanceTo(t2.center);
        if((t1.isTurnRight && !t2.isTurnRight) || (!t1.isTurnRight && t2.isTurnRight))
            if(distance < 2 * R)
                return;

        qreal length = 0;
        AligmentResult result;
        if((t1.isTurnRight && !t2.isTurnRight) || (!t1.isTurnRight && t2.isTurnRight))
            result = CalculateTwoSideLine(t1, t2, R, length);
        else
            result = CalculateOneSideLine(t1, t2, R, length);

        maneuverLengths[length] = result;
    };
    addTurnPairsChecked(t1R, t2R);
    addTurnPairsChecked(t1L, t2R);
    addTurnPairsChecked(t1R, t2L);
    addTurnPairsChecked(t1L, t2L);

    if(maneuverLengths.empty())
        return GeoPoints();

    flightAligment_ = maneuverLengths.begin()->second.flightPoints;
    return maneuverLengths.begin()->second.points;
}

ManeuverTrackAlignment::TurnPointData ManeuverTrackAlignment::GetTurnPointData(qreal R, const GeoPoint &pnt, Azimuth az, bool isTurnRight)
{
    auto turnPoint = pnt.atDistanceAndAzimuth(R, az + (isTurnRight ? 90 : 270));
    return TurnPointData{turnPoint, isTurnRight};
}

template<typename TTurnInfo>
ManeuverTrackAlignment::AligmentResult ManeuverTrackAlignment::CalculateOneSideLine(TTurnInfo &&turn1, TTurnInfo &&turn2, qreal R, qreal &length)
{
    auto az = Azimuth(turn1.center.azimuthTo(turn2.center) + (turn1.isTurnRight ? 270 : 90));
    auto t1mnv = turn1.center.atDistanceAndAzimuth(R, az);
    auto t2mnv = turn2.center.atDistanceAndAzimuth(R, az);

    AligmentResult result;
    result.flightPoints.push_back(t1mnv);
    result.flightPoints.push_back(t2mnv);

    auto azt1p1 = turn1.center.azimuthTo(pnt1_);
    auto azt1t1mnv = az;
    AddManeuverTurn(result.points, R, turn1, azt1p1, azt1t1mnv);
    length += CalculateManeuverLength(R, turn1.isTurnRight, azt1p1, azt1t1mnv);

    result.points.push_back(t1mnv);
    result.points.push_back(t2mnv);
    length += t1mnv.distanceTo(t2mnv);

    auto azt2p1 = turn2.center.azimuthTo(pnt2_);
    auto azt2t2mnv = az;
    AddManeuverTurn(result.points, R, turn2, azt2t2mnv, azt2p1);
    length += CalculateManeuverLength(R, turn2.isTurnRight, azt2t2mnv, azt2p1);

    return result;
}

template<typename TTurnInfo>
ManeuverTrackAlignment::AligmentResult ManeuverTrackAlignment::CalculateTwoSideLine(TTurnInfo &&turn1, TTurnInfo &&turn2, qreal R, qreal &length)
{
    auto halfDistance = turn1.center.distanceTo(turn2.center) / 2.0;
    auto deltaAz = qRadiansToDegrees(acos(R / halfDistance));
    auto az = turn1.center.azimuthTo(turn2.center) + (turn1.isTurnRight ? -deltaAz : deltaAz);
    auto t1mnv = turn1.center.atDistanceAndAzimuth(R, az);
    auto t2mnv = turn2.center.atDistanceAndAzimuth(R, az + 180);

    AligmentResult result;
    result.flightPoints.push_back(t1mnv);
    result.flightPoints.push_back(t2mnv);

    auto azt1p1 = turn1.center.azimuthTo(pnt1_);
    auto azt1t1mnv = az;
    AddManeuverTurn(result.points, R, turn1, azt1p1, azt1t1mnv);
    length += CalculateManeuverLength(R, turn1.isTurnRight, azt1p1, azt1t1mnv);

    result.points.push_back(t1mnv);
    result.points.push_back(t2mnv);
    length += t1mnv.distanceTo(t2mnv);

    auto azt2p1 = turn2.center.azimuthTo(pnt2_);
    auto azt2t2mnv = az + 180;
    AddManeuverTurn(result.points, R, turn2, azt2t2mnv, azt2p1);
    length += CalculateManeuverLength(R, turn2.isTurnRight, azt2t2mnv, azt2p1);

    return result;
}

template<typename TTurnInfo>
void ManeuverTrackAlignment::AddManeuverTurn(GeoPoints &points, qreal R, TTurnInfo &&turn, Azimuth az1A, Azimuth az2A)
{
    if(az1A == az2A)
        return;

    auto addPointsInternal = [&points, R](const GeoPoint &center, auto az1, auto az2, auto delta, auto comparer) {
        for(; comparer(az1, az2); az1 += delta) { // (turn.isTurnRight ? (az < az2) : (az > az2)); az += delta) {
            points.push_back(center.atDistanceAndAzimuth(R, az1));
        }
    };

    const auto delta = 10;
    Azimuth::ValueType az1 = az1A;
    if(az1 < 0)
        az1 += 360;
    Azimuth::ValueType az2 = az2A;
    if(az2 < 0)
        az2 += 360;

    if(turn.isTurnRight) {
        if(az2 < az1)
            az2 += 360;
        addPointsInternal(turn.center, az1, az2, delta, [](auto cur, auto end) { return cur < end; });
    } else {
        if(az2 > az1)
            az2 -= 360;
        addPointsInternal(turn.center, az1, az2, -delta, [](auto cur, auto end) { return cur > end; });
    }
}

void ManeuverTrackAlignment::AddCircle(GeoPoints &points, const GeoPoint &center, qreal R)
{
    for(qreal az = 0; az < 360; az += 30)
        points.push_back(center.atDistanceAndAzimuth(R, az));
    points.push_back(points.front());
}

qreal ManeuverTrackAlignment::CalculateManeuverLength(qreal R, bool isClockwise, Azimuth az1A, Azimuth az2A)
{
    Azimuth::ValueType az1 = az1A;
    if(az1 < 0)
        az1 += 360;
    Azimuth::ValueType az2 = az2A;
    if(az2 < 0)
        az2 += 360;

    Azimuth::ValueType delta = 0;
    if(isClockwise) {
        if(az2 < az1)
            az2 += 360;
        delta = az2 - az1;
    } else {
        if(az2 > az1)
            az2 -= 360;
        delta = az1 - az2;
    }
    return M_PI * R * delta / 180;
}

} // namespace aero_photo

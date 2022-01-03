#pragma once

#include "PhotoPlannerCore.h"
#include "Azimuth.h"
#include "PhotoUavModel.h"
#include "PhotoGeoCalcs.h"

#include <map>

namespace aero_photo
{

class ManeuverTrackAlignment
{

    struct TurnPointData {
        GeoPoint center;
        bool isTurnRight = true;
    };

    struct AligmentResult {
        GeoPoints points;
        GeoPoints flightPoints;
    };

public:
    ManeuverTrackAlignment(const GeoPoint &pnt1, qreal az1, const GeoPoint &pnt2, qreal az2);
    GeoPoints Calculate(const PhotoUavModel &uavModel);
    GeoPoints GetFlightPoints() const;

private:
    GeoPoints CalculateMoreThen2RAligment(qreal R);
    static TurnPointData GetTurnPointData(qreal R, const GeoPoint &pnt, Azimuth az, bool isTurnRight);
    template<typename TTurnInfo>
    AligmentResult CalculateOneSideLine(TTurnInfo &&turn1, TTurnInfo &&turn2, qreal R, qreal &length);
    template<typename TTurnInfo>
    AligmentResult CalculateTwoSideLine(TTurnInfo &&turn1, TTurnInfo &&turn2, qreal R, qreal &length);
    template<typename TTurnInfo>
    void AddManeuverTurn(GeoPoints &points, qreal R, TTurnInfo &&turn, Azimuth az1A, Azimuth az2A);
    void AddCircle(GeoPoints &points, const GeoPoint &center, qreal R);
    static qreal CalculateManeuverLength(qreal R, bool isClockwise, Azimuth az1A, Azimuth az2A);

    const GeoPoint pnt1_;
    const Azimuth az1_;
    const GeoPoint pnt2_;
    const Azimuth az2_;

    const qreal distance12_;
    const Azimuth az12_;

    GeoPoints flightAligment_;
};

} // namespace aero_photo

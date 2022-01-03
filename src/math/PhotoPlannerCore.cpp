#include "PhotoPlannerCore.h"

namespace aero_photo
{

PhotoPrint::PhotoPrint(const GeoPoint &center, const GeoPoints &border):
    center_(center), border_(border)
{
}

const GeoPoint &PhotoPrint::GetCenter() const
{
    return center_;
}
const GeoPoints &PhotoPrint::GetBorder() const
{
    return border_;
}

FlightPoint::FlightPoint(const GeoPoint &geoPoint, int type, qreal shotDistance):
    GeoPoint(geoPoint), type_(type), shotDistance_(shotDistance)
{
}

int FlightPoint::type() const
{
    return type_;
}
qreal FlightPoint::shotDistance() const
{
    return shotDistance_;
}

LinearPhotoRegion::LinearPhotoRegion(const GeoPoints &track):
    track_(track)
{
}

const GeoPoints &LinearPhotoRegion::GetTrack() const
{
    return track_;
}

AreaPhotoRegion::AreaPhotoRegion(const GeoPoints &border):
    border_(border)
{
}

const GeoPoints &AreaPhotoRegion::GetBorder() const
{
    return border_;
}

CalculationParams &CalculationParams::Instance()
{
    static CalculationParams instance;
    return instance;
}
} // namespace aero_photo

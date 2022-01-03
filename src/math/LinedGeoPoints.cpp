#include "LinedGeoPoints.h"

namespace aero_photo
{
PlannedTrackLine::PlannedTrackLine(const GeoPoints &photoPrints, size_t adjustments):
    GeoPoints(photoPrints), adjustments_(adjustments)
{
    if(size() > 2)
        azimuth_ = front().azimuthTo(back());
}

size_t PlannedTrackLine::GetAdjustments() const
{
    return adjustments_;
}
qreal PlannedTrackLine::GetAzimuth() const
{
    return azimuth_;
}

} // namespace aero_photo
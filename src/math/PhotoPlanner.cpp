#include "PhotoPlanner.h"

namespace aero_photo
{

PhotoPlanner::PhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera):
    photoUav_(photoUav), photoCamera_(photoCamera)
{
}

bool PhotoPlanner::IsCalculated() const
{
    return isCalculated;
}
const PhotoPrints &PhotoPlanner::GetPhotoPrints() const
{
    return photoPrints_;
}
const GeoPoints &PhotoPlanner::GetTrackPoints() const
{
    return trackPoints_;
}
const FlightPoints &PhotoPlanner::GetFlightPoints() const
{
    return flightPoints_;
}

qreal PhotoPlanner::velocity() const
{
    return photoUav_.velocity();
}

void PhotoPlanner::SaveToXml(QString fileurlcvt, const int totalPointsInPacket)
{
    PhotoPlannerXmlWriter writer(GetFlightPoints(), velocity());
    writer.WritePackets(fileurlcvt, totalPointsInPacket);
}

void PhotoPlanner::CalculateTrack(double Bx)
{
    trackPoints_.clear();
    flightPoints_.clear();

    const auto R = photoUav_.GetManeuverR();

    int prevLine = -1;
    for(int i = 0; i < linedGeoPoints_.size(); ++i) {
        auto &line = linedGeoPoints_[i];
        if(line.empty())
            continue;

        const bool enlargeEntryRequired = (R != 0) &&
                                          CalculationParams::Instance().enlargeEntryRequired &&
                                          (linedGeoPoints_[i].GetAdjustments() & PlannedTrackLine::AdjustmentEnlargeEntry);

        auto endAligmentPoint = linedGeoPoints_[i].front();
        auto additionalEntryStart = endAligmentPoint.atDistanceAndAzimuth(4 * R, linedGeoPoints_[i].GetAzimuth() + 180);
        auto additionalEntryEnd = endAligmentPoint.atDistanceAndAzimuth(2 * R, linedGeoPoints_[i].GetAzimuth() + 180);
        if(enlargeEntryRequired) {
            endAligmentPoint = additionalEntryStart;
        }

        if(prevLine >= 0) {
            auto aligmentMode = linedGeoPoints_[i].GetAdjustments() & PlannedTrackLine::AlignmentsMask;
            switch(aligmentMode) {
            case PlannedTrackLine::AdjustmentByUav: {
                if(!trackPoints_.empty() && !linedGeoPoints_[i].empty() &&
                   linedGeoPoints_[i].front().distanceTo(trackPoints_.back()) < 10) {
                    trackPoints_.pop_back();
                    flightPoints_.pop_back();
                }
                break;
            }
            case PlannedTrackLine::AdjustmentAlignByManeuver: {
                ManeuverTrackAlignment aligment(linedGeoPoints_[prevLine].back(), linedGeoPoints_[prevLine].GetAzimuth(), endAligmentPoint, linedGeoPoints_[i].GetAzimuth());
                auto aligmentPoints = aligment.Calculate(photoUav_);
                for(auto point: aligmentPoints)
                    trackPoints_.push_back(point);
                for(auto point: aligment.GetFlightPoints())
                    flightPoints_.push_back(FlightPoint(point, 0));
                break;
            }
            }
        }

        if(enlargeEntryRequired) {
            trackPoints_.push_back(additionalEntryStart);
            flightPoints_.push_back(FlightPoint(additionalEntryStart, 0));
        }

        trackPoints_.push_back(line.front());
        flightPoints_.push_back(FlightPoint(line.front(), 1, Bx));
        if(line.size() > 1) {
            trackPoints_.push_back(line.back());
            flightPoints_.push_back(FlightPoint(line.back(), 1));
        }
        prevLine = i;
    }

    isCalculated = true;
}

} // namespace aero_photo

#pragma once

#include "PhotoPlannerCore.h"
#include "PhotoCameraModel.h"
#include "PhotoUavModel.h"
#include "LinedGeoPoints.h"
#include "ManeuverTrackAlignment.h"
#include "PhotoPlannerXmlWriter.h"

namespace aero_photo
{

class PhotoPlanner
{
protected:
    PhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera);

public:
    virtual ~PhotoPlanner() = default;

    bool IsCalculated() const;
    const PhotoPrints &GetPhotoPrints() const;
    const GeoPoints &GetTrackPoints() const;
    const FlightPoints &GetFlightPoints() const;

    qreal velocity() const;

    void SaveToXml(QString fileurlcvt, const int totalPointsInPacket);

protected:
    void CalculateTrack(double Bx);

    bool isCalculated = false;
    PhotoUavModel photoUav_;
    PhotoCameraModel photoCamera_;
    LinedGeoPoints linedGeoPoints_;
    GeoPoints trackPoints_;
    FlightPoints flightPoints_;
    PhotoPrints photoPrints_;
};

} // namespace aero_photo

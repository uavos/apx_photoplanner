#pragma once

#include "PhotoPlannerCore.h"
#include "PhotoGeoCalcs.h"

namespace aero_photo
{

class LinePhotoPrintsGenerator
{
public:
    LinePhotoPrintsGenerator(const GeoPoint &pntA, qreal azimuth, GeoDistance distance);
    LinePhotoPrintsGenerator(const GeoPoint &pntA, const GeoPoint &pntB);
    GeoPoints GeneratePhotoPrintsCenters(qreal LxpMax);
    PhotoPrints GeneratePhotoPrints(const GeoPoints &photoPrintsCenters, qreal Lx, qreal Ly);
    PhotoPrints GeneratePhotoPrints(qreal Lxp, qreal Lx, qreal Ly);

private:
    const GeoPoint pntA_;
    const qreal azimuth_;
    const qreal distance_;
};

} // namespace aero_photo

#pragma once

#include "PhotoPlannerCore.h"
#include "LinedGeoPoints.h"
#include "LinePhotoPrintsGenerator.h"

#include <stdexcept>
#include <cassert>

namespace aero_photo {

class LinearPhotoPrintsGenerator {
public:
  explicit LinearPhotoPrintsGenerator(const LinearPhotoRegion &linearRegion);
  LinedGeoPoints GeneratePhotoPrintsCenters(qreal h, qreal Lxp, qreal Lyp, size_t totalRuns, const qreal tolerateAngle = 0);
  PhotoPrints GeneratePhotoPrints(const LinedGeoPoints &linedGeoPoints, qreal Lx, qreal Ly);
  int CalculateRunSequnce(int trackIndex, int runIndex);

private:
  GeoPoint trackHead_;
  GeoPoints trackTail_;
};

} // aero_photo

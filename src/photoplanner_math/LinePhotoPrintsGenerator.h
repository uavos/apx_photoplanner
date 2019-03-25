#ifndef LINEPHOTOPRINTSGENERATOR_H
#define LINEPHOTOPRINTSGENERATOR_H

#include "PhotoPlannerCore.h"
#include "PhotoGeoCalcs.h"

namespace aero_photo {

class LinePhotoPrintsGenerator {
public:
  LinePhotoPrintsGenerator(const GeoPoint &pntA, qreal azimuth, GeoDistance distance)
    : pntA_(pntA)
    , azimuth_(azimuth)
    , distance_(distance) {
  }

  LinePhotoPrintsGenerator(const GeoPoint &pntA, const GeoPoint &pntB)
    : LinePhotoPrintsGenerator(pntA, pntA.azimuthTo(pntB), pntA.distanceTo(pntB)) {
  }

//  GeoPoints GeneratePhotoPrintsCenters(qreal Lxp) {
//    GeoPoints photoPrintsCenters;
//    const int totalPrints = ceil(distance_ / Lxp) + 1;
//    for (int i = 0; i < totalPrints; ++i) {
//      auto nextCenter = pntA_.atDistanceAndAzimuth(Lxp*i, azimuth_);
//      photoPrintsCenters.push_back(nextCenter);
//    }
//    return photoPrintsCenters;
//  }

  GeoPoints GeneratePhotoPrintsCenters(qreal LxpMax) {
    GeoPoints photoPrintsCenters;
    const int totalPrints = ceil(distance_ / LxpMax) + 1;
    auto Lxp = distance_ / (totalPrints - 1);
    for (int i = 0; i < totalPrints; ++i) {
      auto nextCenter = pntA_.atDistanceAndAzimuth(Lxp*i, azimuth_);
      photoPrintsCenters.push_back(nextCenter);
    }
    return photoPrintsCenters;
  }
  PhotoPrints GeneratePhotoPrints(const GeoPoints &photoPrintsCenters, qreal Lx, qreal Ly) {
    PhotoPrints linePhotoPrints;
    auto photoPrintHalfDiag = sqrt(Lx*Lx + Ly*Ly) / 2.0;
    auto cornerAngle = R2D(atan(Ly / Lx));
    auto generatePhotoPrintBorder = [this, photoPrintHalfDiag, cornerAngle](const GeoPoint &photoPrintCenter) {
      auto ppLT = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ - cornerAngle);
      auto ppRT = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ + cornerAngle);
      auto ppLB = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ + 180 + cornerAngle);
      auto ppRB = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ + 180 - cornerAngle);
      GeoPoints photoPrintBorder = {ppLT, ppRT, ppLB, ppRB};
      return photoPrintBorder;
    };
    for (auto &&photoPrintCenter : photoPrintsCenters) {
      linePhotoPrints << PhotoPrint(photoPrintCenter, generatePhotoPrintBorder(photoPrintCenter));
    }
    return linePhotoPrints;
  }
  PhotoPrints GeneratePhotoPrints(qreal Lxp, qreal Lx, qreal Ly) {
    auto photoPrintsCenters = GeneratePhotoPrintsCenters(Lxp);
    return GeneratePhotoPrints(photoPrintsCenters, Lx, Ly);
  }

private:
  const GeoPoint pntA_;
  const qreal azimuth_;
  const qreal distance_;
};

} // aero_photo

#endif // LINEPHOTOPRINTSGENERATOR_H

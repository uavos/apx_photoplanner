#include "LinePhotoPrintsGenerator.h"

namespace aero_photo
{

LinePhotoPrintsGenerator::LinePhotoPrintsGenerator(const GeoPoint &pntA, qreal azimuth, GeoDistance distance):
    pntA_(pntA), azimuth_(azimuth), distance_(distance)
{
}

LinePhotoPrintsGenerator::LinePhotoPrintsGenerator(const GeoPoint &pntA, const GeoPoint &pntB):
    LinePhotoPrintsGenerator(pntA, pntA.azimuthTo(pntB), pntA.distanceTo(pntB))
{
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

GeoPoints LinePhotoPrintsGenerator::GeneratePhotoPrintsCenters(qreal LxpMax)
{
    GeoPoints photoPrintsCenters;
    const int totalPrints = ceil(distance_ / LxpMax) + 1;
    auto Lxp = distance_ / (totalPrints - 1);
    for(int i = 0; i < totalPrints; ++i) {
        auto nextCenter = pntA_.atDistanceAndAzimuth(Lxp * i, azimuth_);
        photoPrintsCenters.push_back(nextCenter);
    }
    return photoPrintsCenters;
}
PhotoPrints LinePhotoPrintsGenerator::GeneratePhotoPrints(const GeoPoints &photoPrintsCenters, qreal Lx, qreal Ly)
{
    PhotoPrints linePhotoPrints;
    auto photoPrintHalfDiag = sqrt(Lx * Lx + Ly * Ly) / 2.0;
    auto cornerAngle = qRadiansToDegrees(atan(Ly / Lx));
    auto generatePhotoPrintBorder = [this, photoPrintHalfDiag, cornerAngle](const GeoPoint &photoPrintCenter) {
        auto ppLT = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ - cornerAngle);
        auto ppRT = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ + cornerAngle);
        auto ppLB = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ + 180 + cornerAngle);
        auto ppRB = photoPrintCenter.atDistanceAndAzimuth(photoPrintHalfDiag, azimuth_ + 180 - cornerAngle);
        GeoPoints photoPrintBorder = {ppLT, ppRT, ppLB, ppRB};
        return photoPrintBorder;
    };
    for(auto &&photoPrintCenter: photoPrintsCenters) {
        linePhotoPrints << PhotoPrint(photoPrintCenter, generatePhotoPrintBorder(photoPrintCenter));
    }
    return linePhotoPrints;
}
PhotoPrints LinePhotoPrintsGenerator::GeneratePhotoPrints(qreal Lxp, qreal Lx, qreal Ly)
{
    auto photoPrintsCenters = GeneratePhotoPrintsCenters(Lxp);
    return GeneratePhotoPrints(photoPrintsCenters, Lx, Ly);
}

} // namespace aero_photo

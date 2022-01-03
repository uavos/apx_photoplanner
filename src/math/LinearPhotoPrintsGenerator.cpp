#include "LinearPhotoPrintsGenerator.h"

namespace aero_photo
{

LinearPhotoPrintsGenerator::LinearPhotoPrintsGenerator(const LinearPhotoRegion &linearRegion)
{
    if(linearRegion.GetTrack().size() < 2)
        throw std::logic_error("More then 1 point must be entered for LINEAR calculation!");
    trackHead_ = linearRegion.GetTrack().front();
    trackTail_ = linearRegion.GetTrack();
    trackTail_.pop_front();
}

LinedGeoPoints LinearPhotoPrintsGenerator::GeneratePhotoPrintsCenters(qreal h, qreal Lxp, qreal Lyp, size_t totalRuns, const qreal tolerateAngle)
{
    assert(!trackTail_.empty());

    trackHead_.setAltitude(h);
    for(auto &tailPnt: trackTail_)
        tailPnt.setAltitude(h);

    LinedGeoPoints linedGeoPoints(totalRuns * (trackTail_.size()));

    // !!! Both pntA & pntB must be copy of members
    auto pntA = trackHead_;
    auto azimuthToA = pntA.azimuthTo(trackTail_[0]);
    for(int trackIndex = 0; trackIndex < trackTail_.size(); trackIndex++) {
        const bool isFirstSegment = (trackIndex == 0);
        const bool isLastSegment = (trackIndex + 1 == trackTail_.size());

        auto pntB = trackTail_[trackIndex];
        const auto azimuthAB = pntA.azimuthTo(pntB);
        const auto azimuthFromB = (!isLastSegment)
                                      ? pntB.azimuthTo(trackTail_[trackIndex + 1])
                                      : azimuthAB;

        const bool fixA = (abs(azimuthAB - azimuthToA) < tolerateAngle) && !isFirstSegment;
        const bool fixB = (abs(azimuthFromB - azimuthAB) < tolerateAngle) && !isLastSegment;
        GeoCalc geoCalc;
        geoCalc.RoundUpPoints(pntA, pntB, Lxp, fixA, fixB);
        RunStartPointsCalc pointsA(azimuthToA, pntA, azimuthAB, Lyp, totalRuns);
        RunStartPointsCalc pointsB(azimuthAB, pntB, azimuthFromB, Lyp, totalRuns);
        for(size_t runIndex = 0; runIndex < totalRuns; runIndex++) {
            auto aSidePair = std::make_pair(pointsA.Calculate(runIndex), fixA);
            auto bSidePair = std::make_pair(pointsB.Calculate(runIndex), fixB);
            const bool isDirectDirection = (runIndex % 2 == 0);
            const auto &startPair = isDirectDirection ? aSidePair : bSidePair;
            const auto &finishPair = isDirectDirection ? bSidePair : aSidePair;

            LinePhotoPrintsGenerator thisRunLineGenerator(startPair.first, finishPair.first);
            auto pointRunPrintsCenters = thisRunLineGenerator.GeneratePhotoPrintsCenters(Lxp);
            // We must calculate index
            const auto linedIndex = CalculateRunSequnce(trackIndex, runIndex);
            const bool adjustedByUavRequired =
                startPair.second &&
                ((isDirectDirection && trackIndex != 0) || (!isDirectDirection && trackIndex + 1 != trackTail_.size())) &&
                true;
            const auto guiAligmentMode = CalculationParams::Instance().maneuverAligmentRequired
                                             ? PlannedTrackLine::AdjustmentAlignByManeuver
                                             : PlannedTrackLine::AdjustmentByUav;
            const auto guiExtendedMode = CalculationParams::Instance().enlargeEntryRequired
                                             ? PlannedTrackLine::AdjustmentEnlargeEntry
                                             : 0;
            const size_t lineAdjustment = (adjustedByUavRequired)
                                              ? PlannedTrackLine::AdjustmentByUav
                                              : guiAligmentMode | guiExtendedMode;
            linedGeoPoints[linedIndex] = PlannedTrackLine(pointRunPrintsCenters, lineAdjustment);
        }
        pntA = trackTail_[trackIndex];
        azimuthToA = azimuthAB;
    }
    return linedGeoPoints;
}

PhotoPrints LinearPhotoPrintsGenerator::GeneratePhotoPrints(const LinedGeoPoints &linedGeoPoints, qreal Lx, qreal Ly)
{
    PhotoPrints photoPrints;
    //photoPrints.reserve(linedGeoPoints.size());
    for(auto &&linePoints: linedGeoPoints) {
        LinePhotoPrintsGenerator thisRunLineGenerator(linePoints.front(), linePoints.back());
        auto thisRunLinePhotoPrints = thisRunLineGenerator.GeneratePhotoPrints(linePoints, Lx, Ly);
        photoPrints.append(thisRunLinePhotoPrints);
    }
    photoPrints.squeeze();
    return photoPrints;
}

int LinearPhotoPrintsGenerator::CalculateRunSequnce(int trackIndex, int runIndex)
{
    const auto totalTracks = trackTail_.size();
    auto sequence =
        ((runIndex + 1) % 2) * trackIndex +
        (runIndex % 2) * (totalTracks - 1 - trackIndex) +
        runIndex * totalTracks;
    return sequence;
}

} // namespace aero_photo
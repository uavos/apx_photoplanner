#pragma once

#include "PhotoPlannerCore.h"

namespace aero_photo
{

class PlannedTrackLine: public GeoPoints
{
public:
    static constexpr size_t AdjustmentByUav = 0x0001;
    static constexpr size_t AdjustmentEnlargeEntry = 0x0002;
    static constexpr size_t AdjustmentAlignByManeuver = 0x0004;
    static constexpr size_t AdjustmentAlignDefault = AdjustmentEnlargeEntry | AdjustmentAlignByManeuver;

    static constexpr size_t AlignmentsMask = AdjustmentByUav | AdjustmentAlignByManeuver;

public:
    explicit PlannedTrackLine() = default;
    explicit PlannedTrackLine(const GeoPoints &photoPrints, size_t adjustments = AdjustmentAlignDefault);
    size_t GetAdjustments() const;
    qreal GetAzimuth() const;

private:
    size_t adjustments_ = AdjustmentAlignDefault;
    qreal azimuth_ = 0;
};

using LinedGeoPoints = QVector<PlannedTrackLine>;

} // namespace aero_photo

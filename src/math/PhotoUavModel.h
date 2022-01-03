#pragma once

#include <QtGlobal>
#include <cmath>

namespace aero_photo
{

class PhotoUavModel
{
public:
    PhotoUavModel(qreal photoVelocityMPerSec, qreal photoRollRadian);

    qreal GetManeuverR() const;
    qreal velocity() const;

    qreal CalcUavMaxRollOnManeuverR(qreal maneuverR);
    qreal CalcUavManeuverROnMaxRoll(qreal photoRollRadian);

private:
    qreal velocity_;
    qreal roll_;
};

} // namespace aero_photo

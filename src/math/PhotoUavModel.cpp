#include "PhotoUavModel.h"

namespace aero_photo
{

PhotoUavModel::PhotoUavModel(qreal photoVelocityMPerSec, qreal photoRollRadian):
    velocity_(photoVelocityMPerSec), roll_(photoRollRadian)
{
}

qreal PhotoUavModel::GetManeuverR() const
{
    return velocity_ * velocity_ / 9.81 / tan(roll_);
}
qreal PhotoUavModel::velocity() const
{
    return velocity_;
}

qreal PhotoUavModel::CalcUavMaxRollOnManeuverR(qreal maneuverR)
{
    return atan(velocity_ * velocity_ / 9.81 / maneuverR);
}
qreal PhotoUavModel::CalcUavManeuverROnMaxRoll(qreal photoRollRadian)
{
    return velocity_ * velocity_ / 9.81 / tan(photoRollRadian);
}

} // namespace aero_photo

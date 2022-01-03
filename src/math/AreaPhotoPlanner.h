#pragma once

#include "PhotoPlannerCore.h"
#include "PhotoPlanner.h"
#include "AreaPhotoPrintsGenerator.h"

namespace aero_photo
{

class AreaPhotoPlanner: public PhotoPlanner
{
public:
    AreaPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const AreaPhotoRegion &photoRegion);
    bool Calculate(double h, double Px, double Py, qreal azimuth, size_t extentBorderValue, bool withPhotoPrints);

private:
    AreaPhotoPrintsGenerator photoPrintsGenerator_;
};

} // namespace aero_photo

#pragma once

#include "PhotoPlannerCore.h"
#include "PhotoPlanner.h"
#include "LinearPhotoPrintsGenerator.h"

namespace aero_photo
{

class LinearPhotoPlanner: public PhotoPlanner
{
public:
    LinearPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const LinearPhotoRegion &photoRegion);
    bool Calculate(double h, double Px, double Py, double width, bool withPhotoPrints);

private:
    LinearPhotoPrintsGenerator photoPrintsGenerator_;
};

} // namespace aero_photo

#ifndef AREAPHOTOPLANNER_H
#define AREAPHOTOPLANNER_H

#include "PhotoPlannerCore.h"
#include "PhotoPlanner.h"
#include "AreaPhotoPrintsGenerator.h"

namespace aero_photo {

class AreaPhotoPlanner : public PhotoPlanner {
public:
    AreaPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const AreaPhotoRegion &photoRegion)
        : PhotoPlanner(photoUav, photoCamera)
        , photoPrintsGenerator_(photoRegion) {
        qDebug() << "Created photo planner for area region: " << photoRegion.GetBorder();
    }

    bool Calculate(double h, double Px, double Py, qreal azimuth, size_t extentBorderValue) {
        isCalculated = false;
        double Bx, By;
        photoCamera_.CalcBxBy(h, Px, Py, Bx, By);
        linedGeoPoints_ = photoPrintsGenerator_.GeneratePhotoPrintsCenters(h, Bx, By, azimuth, extentBorderValue);
        double Lx, Ly;
        photoCamera_.CalcLxLy(h, Lx, Ly);
        photoPrints_ = photoPrintsGenerator_.GeneratePhotoPrints(linedGeoPoints_, Lx, Ly);

        CalculateTrack(Bx);

        return IsCalculated();
    }

private:
    AreaPhotoPrintsGenerator photoPrintsGenerator_;
};

} // aero_photo

#endif // AREAPHOTOPLANNER_H


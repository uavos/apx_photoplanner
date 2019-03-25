#ifndef LINEARPHOTOPLANNER_H
#define LINEARPHOTOPLANNER_H

#include "PhotoPlannerCore.h"
#include "PhotoPlanner.h"
#include "LinearPhotoPrintsGenerator.h"

namespace aero_photo {

class LinearPhotoPlanner : public PhotoPlanner {
public:
    LinearPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const LinearPhotoRegion &photoRegion)
        : PhotoPlanner(photoUav, photoCamera)
        , photoPrintsGenerator_(photoRegion) {
        qDebug() << "Created photo planner for linear region: " << photoRegion.GetTrack();
    }

    bool Calculate(double h, double Px, double Py, double width) {
        isCalculated = false;
        double Bx, By;
        photoCamera_.CalcBxBy(h, Px, Py, Bx, By);
        auto totalRuns = ceil(width/By);
        linedGeoPoints_ = photoPrintsGenerator_.GeneratePhotoPrintsCenters(h, Bx, By, totalRuns, 10);
        double Lx, Ly;
        photoCamera_.CalcLxLy(h, Lx, Ly);
        photoPrints_ = photoPrintsGenerator_.GeneratePhotoPrints(linedGeoPoints_, Lx, Ly);
        CalculateTrack(Bx);
        return IsCalculated();
    }

private:
    LinearPhotoPrintsGenerator photoPrintsGenerator_;
};

} // aero_photo

#endif // LINEARPHOTOPLANNER_H

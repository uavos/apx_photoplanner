#include "LinearPhotoPlanner.h"

namespace aero_photo
{

LinearPhotoPlanner::LinearPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const LinearPhotoRegion &photoRegion):
    PhotoPlanner(photoUav, photoCamera), photoPrintsGenerator_(photoRegion)
{
    qDebug() << "Created photo planner for linear region: " << photoRegion.GetTrack();
}

bool LinearPhotoPlanner::Calculate(double h, double Px, double Py, double width, bool withPhotoPrints)
{
    isCalculated = false;
    double Bx, By;
    photoCamera_.CalcBxBy(h, Px, Py, Bx, By);
    auto totalRuns = ceil(width / By);
    linedGeoPoints_ = photoPrintsGenerator_.GeneratePhotoPrintsCenters(h, Bx, By, totalRuns, 10);
    if(withPhotoPrints) {
        double Lx, Ly;
        photoCamera_.CalcLxLy(h, Lx, Ly);
        photoPrints_ = photoPrintsGenerator_.GeneratePhotoPrints(linedGeoPoints_, Lx, Ly);
    }
    CalculateTrack(Bx);
    return IsCalculated();
}

} // namespace aero_photo
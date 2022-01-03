#include "AreaPhotoPlanner.h"

namespace aero_photo
{

AreaPhotoPlanner::AreaPhotoPlanner(const PhotoUavModel &photoUav, const PhotoCameraModel &photoCamera, const AreaPhotoRegion &photoRegion):
    PhotoPlanner(photoUav, photoCamera), photoPrintsGenerator_(photoRegion)
{
    qDebug() << "Created photo planner for area region: " << photoRegion.GetBorder();
}

bool AreaPhotoPlanner::Calculate(double h, double Px, double Py, qreal azimuth, size_t extentBorderValue, bool withPhotoPrints)
{
    isCalculated = false;
    double Bx, By;
    photoCamera_.CalcBxBy(h, Px, Py, Bx, By);
    linedGeoPoints_ = photoPrintsGenerator_.GeneratePhotoPrintsCenters(h, Bx, By, azimuth, extentBorderValue);
    if(withPhotoPrints) {
        double Lx, Ly;
        photoCamera_.CalcLxLy(h, Lx, Ly);
        photoPrints_ = photoPrintsGenerator_.GeneratePhotoPrints(linedGeoPoints_, Lx, Ly);
    }
    CalculateTrack(Bx);
    return IsCalculated();
}

} // namespace aero_photo
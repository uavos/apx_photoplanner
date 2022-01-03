#include "PhotoCameraModel.h"

namespace aero_photo
{

PhotoCameraModel::PhotoCameraModel(double f, double lx, double ly, double ax, double ay):
    f_(f), lx_(lx), ly_(ly), ax_(ax), ay_(ay)
{
}

void PhotoCameraModel::CalcLxLy(double h, double &Lx, double &Ly) const
{
    const double m = CalcM(h);
    Lx = lx_ * m;
    Ly = ly_ * m;
}

void PhotoCameraModel::CalcBxBy(double h, double Px, double Py, double &Bx, double &By) const
{
    CalcLxLy(h, Bx, By);
    Bx *= (100.0 - Px) / 100.0;
    By *= (100.0 - Py) / 100.0;
}

double PhotoCameraModel::CalcGsd(double h) const
{
    return CalcDpi() * CalcM(h);
}

double PhotoCameraModel::CalcH(double gsd) const
{
    return gsd * f_ / CalcDpi();
}

double PhotoCameraModel::CalcLinearWidth(double h, double Px, double Py, int totalRuns) const
{
    double Bx, By;
    CalcBxBy(h, Px, Py, Bx, By);
    return totalRuns * By - 1;
}

double PhotoCameraModel::CalcLinearRuns(double h, double Px, double Py, int width) const
{
    double Bx, By;
    CalcBxBy(h, Px, Py, Bx, By);
    auto totalRuns = std::ceil(width / By);
    return totalRuns;
}

double PhotoCameraModel::CalcM(double h) const
{
    return h / f_;
}
double PhotoCameraModel::CalcDpi() const
{
    return lx_ / ax_;
}

} // namespace aero_photo

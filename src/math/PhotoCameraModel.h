#pragma once

#include <cmath>

namespace aero_photo {

class PhotoCameraModel {
public:
    PhotoCameraModel(double f, double lx, double ly, double ax, double ay);
    void CalcLxLy(double h, double &Lx, double &Ly) const;
    void CalcBxBy(double h, double Px, double Py, double &Bx, double &By) const;
    double CalcGsd(double h) const;
    double CalcH(double gsd) const;
    double CalcLinearWidth(double h, double Px, double Py, int totalRuns) const;
    double CalcLinearRuns(double h, double Px, double Py, int width) const;

private:
    double CalcM(double h) const;
    double CalcDpi() const;

    const double f_;
    const double lx_, ly_;
    const double ax_, ay_;
};

} // aero_photo

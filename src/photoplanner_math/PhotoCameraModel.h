#ifndef PHOTOCAMERAMODEL_H
#define PHOTOCAMERAMODEL_H

#include <cmath>

namespace aero_photo {

class PhotoCameraModel {
public:
    PhotoCameraModel(double f, double lx, double ly, double ax, double ay)
        : f_(f), lx_(lx), ly_(ly), ax_(ax), ay_(ay) {
    }

    void CalcLxLy(double h, double &Lx, double &Ly) const {
        const double m = CalcM(h);
        Lx = lx_ * m;
        Ly = ly_ * m;
    }

    void CalcBxBy(double h, double Px, double Py, double &Bx, double &By) const {
        CalcLxLy(h, Bx, By);
        Bx *= (100.0 - Px) / 100.0;
        By *= (100.0 - Py) / 100.0;
    }

    double CalcGsd(double h) const {
        return CalcDpi() * CalcM(h);
    }

    double CalcH(double gsd) const {
        return gsd * f_ / CalcDpi();
    }

    double CalcLinearWidth(double h, double Px, double Py, int totalRuns) const {
        double Bx, By;
        CalcBxBy(h, Px, Py, Bx, By);
        return totalRuns * By - 1;
    }

    double CalcLinearRuns(double h, double Px, double Py, int width) const {
        double Bx, By;
        CalcBxBy(h, Px, Py, Bx, By);
        auto totalRuns = std::ceil(width/By);
        return totalRuns;
    }

private:
    inline double CalcM(double h) const {
        return h/f_;
    }
    inline double CalcDpi() const {
        return lx_/ax_;
    }

    const double f_;
    const double lx_, ly_;
    const double ax_, ay_;
};

} // aero_photo

#endif // PHOTOCAMERAMODEL_H

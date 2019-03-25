#ifndef PHOTOGEOUCALC_H
#define PHOTOGEOUCALC_H

#include "PhotoPlannerCore.h"
#include <math.h>
#include <cmath>
#include <QPointF>
#include "Azimuth.h"

namespace aero_photo {

constexpr qreal D2R(qreal degree) { return degree * M_PI / qreal(180); }
constexpr qreal R2D(qreal radian) { return radian / M_PI * qreal(180); }

using GeoDistance = qreal;

class GeoCalc {
public:
    qreal Azimuth(const GeoPoint &pntA, const GeoPoint &pntB) {
        return pntA.azimuthTo(pntB);
    }
    GeoDistance Distance(const GeoPoint &pntA, const GeoPoint &pntB) {
        return pntA.distanceTo(pntB);
    }
    GeoPoint AtDistanceAndAzimuth(const GeoPoint &pnt, GeoDistance distance, qreal azimuth) {
        return pnt.atDistanceAndAzimuth(distance, azimuth);
    }

    GeoDistance RoundUpPoints(GeoPoint &pntA, GeoPoint &pntB, GeoDistance factor, bool fixA = false, bool fixB = false) {
        auto distance = Distance(pntA, pntB);
        if(fixA && fixB)
            return distance;

        auto deltaDistance = ceil(distance/factor)*factor - distance;
        if (deltaDistance != 0) {
            auto azimuthAB = Azimuth(pntA, pntB);
            auto azimuthBA = azimuthAB + 180;
            distance += deltaDistance;
            if (!fixA && !fixB) {
                pntA = AtDistanceAndAzimuth(pntA, deltaDistance / 2, azimuthBA);
                pntB = AtDistanceAndAzimuth(pntA, distance, azimuthAB);
            } else if(fixA) {
                pntB = AtDistanceAndAzimuth(pntA, distance, azimuthAB);
            } else { // if (fixB)
                pntA = AtDistanceAndAzimuth(pntB, distance, azimuthBA);
            }
        }
        return distance;
    }
};

class RunStartPointsCalc {
public:
    RunStartPointsCalc(qreal enterAzimuth, const GeoPoint &baseRunPoint, qreal runAzimuth, qreal Lyp, size_t totalRuns)
        : basePoint_(baseRunPoint)
        , startPointsAzimuth_(enterAzimuth + 90 + (runAzimuth - enterAzimuth)/2)
        , Lyp_(Lyp/cos(D2R((runAzimuth - enterAzimuth)/2)))
        , indexOffset_(totalRuns / 2 - totalRuns + 1)
    {
    }

    GeoPoint Calculate(int index, qreal offset = 0) {
        // Indexes from left to right in azimuth direction
        auto startPoint = basePoint_.atDistanceAndAzimuth(Lyp_ * (indexOffset_+ index) + offset, startPointsAzimuth_);
        return startPoint;
    }

private:
    const GeoPoint basePoint_;
    const Azimuth startPointsAzimuth_;
    const qreal Lyp_;
    const int indexOffset_;
};

class CartesianCalcs {
public:
    CartesianCalcs(GeoPoint basePoint, qreal baseAzimuth)
        : basePoint_(basePoint)
        , baseAzimuth_(baseAzimuth)
    {
    }

    QPointF ConvertToCartesian(const GeoPoint &point) {
        auto azimuthToPoint = D2R(basePoint_.azimuthTo(point));
        auto distanceToPoint = basePoint_.distanceTo(point);
        return QPointF(distanceToPoint * cos(azimuthToPoint), distanceToPoint * sin(azimuthToPoint));
    }

private:
    const GeoPoint basePoint_;
    const qreal baseAzimuth_;
};

}

#endif // PHOTOGEOUCALC_H

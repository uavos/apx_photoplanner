#pragma once

#include "PhotoPlannerCore.h"
#include <math.h>
#include <cmath>
#include <QPointF>
#include "Azimuth.h"
#include <QtMath>

namespace aero_photo
{

using GeoDistance = qreal;

class GeoCalc
{
public:
    qreal Azimuth(const GeoPoint &pntA, const GeoPoint &pntB);
    GeoDistance Distance(const GeoPoint &pntA, const GeoPoint &pntB);
    GeoPoint AtDistanceAndAzimuth(const GeoPoint &pnt, GeoDistance distance, qreal azimuth);
    GeoDistance RoundUpPoints(GeoPoint &pntA, GeoPoint &pntB, GeoDistance factor, bool fixA = false, bool fixB = false);
};

class RunStartPointsCalc
{
public:
    RunStartPointsCalc(qreal enterAzimuth, const GeoPoint &baseRunPoint, qreal runAzimuth, qreal Lyp, size_t totalRuns);
    GeoPoint Calculate(int index, qreal offset = 0);

private:
    const GeoPoint basePoint_;
    const Azimuth startPointsAzimuth_;
    const qreal Lyp_;
    const int indexOffset_;
};

class CartesianCalcs
{
public:
    CartesianCalcs(GeoPoint basePoint, qreal baseAzimuth);
    QPointF ConvertToCartesian(const GeoPoint &point);

private:
    const GeoPoint basePoint_;
    const qreal baseAzimuth_;
};

} // namespace aero_photo

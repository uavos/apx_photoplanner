#pragma once

#include "PhotoPlannerCore.h"
#include "LinedGeoPoints.h"
#include "LinePhotoPrintsGenerator.h"

#include "Array2d.h"
#include "NearFilter.h"

#include <vector>
#include <stdint.h>

#include <QPolygonF>
#include <QRectF>

namespace aero_photo
{

class GeoPointsRow: public GeoPoints
{
public:
    explicit GeoPointsRow() = default;
    explicit GeoPointsRow(GeoPoints &&geoPoints, bool isDirectOrder);
    bool IsDirectOrder() const;

private:
    bool isDirectOrder_ = true;
};

class GeoPointsGrid
{
public:
    explicit GeoPointsGrid(size_t totalRuns);
    bool Empty() const;
    bool HasDifferentSizes() const;
    void SetRow(size_t runIndex, GeoPoints &&geoPoints, bool isDirectOrder);
    bool IsRowDirectOrder(size_t rowIndex) const;
    size_t Rows() const;
    size_t Cols() const;
    auto GetGeoPoint(size_t rowIndex, size_t colIndex) const;

private:
    QVector<GeoPointsRow> pointsData_;
};

class RegionInternals
{
public:
    explicit RegionInternals(const AreaPhotoRegion &area);
    auto GetCenter() const;
    auto GetRadius() const;
    auto GetPreferredAzimuth() const;
    LinedGeoPoints GeneratePhotoPrintsCenters(qreal h, qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue);

private:
    void InitializeAreaPolygonAndBounds(const GeoPoints &border);
    void InitializeGeoItems(const GeoPoints &border);
    bool Contains(const GeoPoint &geoPoint) const;

    QPolygonF areaPolygon_;
    QRectF areaBounds_;

    GeoPoint geoCenter_;
    qreal geoRadius_ = 0;

    qreal preferredAzimuth_ = 0;

private:
    GeoPointsGrid GeneratePhotoPrintsGrid(qreal h, qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue);
    LinedGeoPoints FilterPointsGrid(const GeoPointsGrid &pointsGrid, size_t extentBorderValue);
};

class AreaPhotoPrintsGenerator
{
public:
    explicit AreaPhotoPrintsGenerator(const AreaPhotoRegion &area);
    LinedGeoPoints GeneratePhotoPrintsCenters(qreal h, qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue = 0);
    PhotoPrints GeneratePhotoPrints(const LinedGeoPoints &linedGeoPoints, qreal Lx, qreal Ly);
    auto GetPreferredAzimuth() const;

private:
    RegionInternals regionInternals_;
};

} // namespace aero_photo

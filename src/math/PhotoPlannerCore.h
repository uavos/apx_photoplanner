#pragma once

#include <QVector>
#include <QGeoCoordinate>
#include <QPoint>
#include <QtDebug>

namespace aero_photo
{

using GeoPoint = QGeoCoordinate; // WGS84
using GeoPoints = QVector<GeoPoint>;
using CartesianPoint = QPointF;

class PhotoPrint
{
public:
    PhotoPrint() = default;
    PhotoPrint(const GeoPoint &center, const GeoPoints &border);
    const GeoPoint &GetCenter() const;
    const GeoPoints &GetBorder() const;

private:
    GeoPoint center_;
    GeoPoints border_;
};

using PhotoPrints = QVector<PhotoPrint>;

class FlightPoint: public GeoPoint
{
public:
    FlightPoint() = default;
    FlightPoint(const GeoPoint &geoPoint, int type, qreal shotDistance = 0);
    int type() const;
    qreal shotDistance() const;

private:
    int type_ = 0;
    qreal shotDistance_ = 0;
};

using FlightPoints = QVector<FlightPoint>;

class LinearPhotoRegion
{
public:
    explicit LinearPhotoRegion(const GeoPoints &track);
    const GeoPoints &GetTrack() const;

private:
    const GeoPoints track_;
};

class AreaPhotoRegion
{
public:
    explicit AreaPhotoRegion(const GeoPoints &border);
    const GeoPoints &GetBorder() const;

private:
    const GeoPoints border_;
};

class CalculationParams
{
    CalculationParams() = default;

public:
    static CalculationParams &Instance();

    bool enlargeEntryRequired = true;
    bool maneuverAligmentRequired = true;
};

} // namespace aero_photo

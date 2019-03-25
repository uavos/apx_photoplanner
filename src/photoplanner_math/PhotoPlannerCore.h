#ifndef PHOTOPLANNERCORE_H
#define PHOTOPLANNERCORE_H

#include <QVector>
#include <QGeoCoordinate>
#include <QPoint>
#include <QtDebug>

namespace aero_photo {

    using GeoPoint = QGeoCoordinate; // WGS84
    using GeoPoints = QVector<GeoPoint>;
    using CartesianPoint = QPointF;


class PhotoPrint {
public:
    PhotoPrint() {}
    PhotoPrint(const GeoPoint &center, const GeoPoints &border)
        : center_(center), border_(border) {
    }

    const GeoPoint &GetCenter() const {
        return center_;
    }
    const GeoPoints &GetBorder() const {
        return border_;
    }

private:
    GeoPoint center_;
    GeoPoints border_;
};

using PhotoPrints = QVector<PhotoPrint>;


class FlightPoint: public GeoPoint {
public:
    FlightPoint() {}
    FlightPoint(const GeoPoint &geoPoint, int type, qreal shotDistance = 0)
        : GeoPoint(geoPoint)
        , type_(type)
        , shotDistance_(shotDistance) {
    }

    int type() const { return type_; }
    qreal shotDistance() const { return shotDistance_; }

private:
    int type_ = 0;
    qreal shotDistance_ = 0;
};

using FlightPoints = QVector<FlightPoint>;


class LinearPhotoRegion {
public:
    explicit LinearPhotoRegion(const GeoPoints &track) : track_(track) { }

    const GeoPoints &GetTrack() const {
        return track_;
    }

private:
    const GeoPoints track_;
};


class AreaPhotoRegion {
public:
    explicit AreaPhotoRegion(const GeoPoints &border) : border_(border) { }

    const GeoPoints &GetBorder() const {
        return border_;
    }

private:
    const GeoPoints border_;
};


class CalculationParams {
    CalculationParams() { }
public:
    static CalculationParams &Instance() {
        static CalculationParams instance;
        return instance;
    }

    bool enlargeEntryRequired = true;
    bool maneuverAligmentRequired = true;
};


}

#endif // PHOTOPLANNERCORE_H

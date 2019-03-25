#ifndef AREAPHOTOPRINTSGENERATOR_H
#define AREAPHOTOPRINTSGENERATOR_H

#include "PhotoPlannerCore.h"
#include "LinedGeoPoints.h"
#include "LinePhotoPrintsGenerator.h"

#include "Array2d.h"
#include "NearFilter.h"

#include <vector>
#include <stdint.h>

#include <QPolygonF>
#include <QRectF>

namespace aero_photo {

class AreaPhotoPrintsGenerator {

    class RegionInternals {
    public:
        explicit RegionInternals(const AreaPhotoRegion &area) {
            if (area.GetBorder().size()<3)
                throw std::logic_error("More then 2 points must be entered for AREA calculation!");

            InitializeAreaPolygonAndBounds(area.GetBorder());
            InitializeGeoItems(area.GetBorder());
        }

        auto GetCenter() const { return geoCenter_; }
        auto GetRadius() const { return geoRadius_; }
        auto GetPreferredAzimuth() const { return preferredAzimuth_; }

        LinedGeoPoints GeneratePhotoPrintsCenters(qreal h, qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue) {
            auto geoPointsGrid = GeneratePhotoPrintsGrid(h, Lxp, Lyp, azimuth, extentBorderValue);
            return FilterPointsGrid(geoPointsGrid, extentBorderValue);
        }

    private:
        void InitializeAreaPolygonAndBounds(const GeoPoints &border) {
            areaPolygon_.reserve(border.size());
            for(auto geoPoint : border) {
              areaPolygon_ << QPointF(geoPoint.latitude(), geoPoint.longitude());
            }
            if (!areaPolygon_.isClosed())
                areaPolygon_ << areaPolygon_.front();
            areaBounds_ = areaPolygon_.boundingRect();
        }
        void InitializeGeoItems(const GeoPoints &border) {
            geoCenter_.setLatitude(areaBounds_.center().x());
            geoCenter_.setLongitude(areaBounds_.center().y());

            for(auto borderPoint : border) {
                auto distanceToBorder = geoCenter_.distanceTo(borderPoint);
                if (geoRadius_ < distanceToBorder) {
                    geoRadius_ = distanceToBorder;
                    preferredAzimuth_ = geoCenter_.azimuthTo(borderPoint);
                }
            }
        }

        bool Contains(const GeoPoint &geoPoint) const {
            CartesianPoint cartPoint(geoPoint.latitude(), geoPoint.longitude());
            return
                    areaBounds_.contains(cartPoint) &&
                    areaPolygon_.containsPoint(cartPoint, Qt::FillRule::WindingFill) &&
                    true;
        }

        QPolygonF areaPolygon_;
        QRectF areaBounds_;

        GeoPoint geoCenter_;
        qreal geoRadius_ = 0;

        qreal preferredAzimuth_ = 0;

    private:

        class GeoPointsGrid {

            class GeoPointsRow : public GeoPoints {
            public:
                explicit GeoPointsRow() {}
                explicit GeoPointsRow(GeoPoints &&geoPoints, bool isDirectOrder)
                    : GeoPoints(geoPoints)
                    , isDirectOrder_(isDirectOrder)
                {
                }

                bool IsDirectOrder() const { return isDirectOrder_; }

            private:
                bool isDirectOrder_ = true;
            };

        public:
            explicit GeoPointsGrid(size_t totalRuns) : pointsData_(totalRuns) { }

            bool Empty() const { return pointsData_.empty() || pointsData_.front().empty(); }
            bool HasDifferentSizes() const {
                const auto rowSize = pointsData_.front().size();
                for (auto &rowGrid : pointsData_)
                    if (rowSize != rowGrid.size())
                        return true;
                return false;
            }

            void SetRow(size_t runIndex, GeoPoints &&geoPoints, bool isDirectOrder) {
                pointsData_[runIndex] = GeoPointsRow(std::move(geoPoints), isDirectOrder);
            }
            bool IsRowDirectOrder(size_t rowIndex) const { return pointsData_[rowIndex].IsDirectOrder(); }

            size_t Rows() const { return pointsData_.size(); }
            size_t Cols() const { return pointsData_.front().size(); }
            auto GetGeoPoint(size_t rowIndex, size_t colIndex) const { return pointsData_[rowIndex][colIndex]; }

        private:
            QVector<GeoPointsRow> pointsData_;
        };

        GeoPointsGrid GeneratePhotoPrintsGrid(qreal h, qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue) {
            size_t totalRuns = ceil(GetRadius() * 2 / Lyp) + extentBorderValue;
            GeoPointsGrid geoPointsGrid(totalRuns);
            auto centerPoint = GetCenter();
            centerPoint.setAltitude(h);
            RunStartPointsCalc middlePointsCalc(azimuth, centerPoint, azimuth, Lyp, totalRuns);
            for (size_t i = 0; i < totalRuns; ++i) {
                auto middlePoint = middlePointsCalc.Calculate(i);
                const auto runAzimuth = azimuth ; // + 180 * ( i % 2 );
                auto endPntA = middlePoint.atDistanceAndAzimuth(GetRadius() + Lxp * extentBorderValue, runAzimuth + 180);
                auto endPntB = middlePoint.atDistanceAndAzimuth(GetRadius() + Lxp * extentBorderValue, runAzimuth);
                GeoCalc geoCalc;
                const auto runDistance = geoCalc.RoundUpPoints(endPntA, endPntB, Lxp);
                LinePhotoPrintsGenerator thisRunLineGenerator(endPntA, endPntA.azimuthTo(endPntB), runDistance);
                geoPointsGrid.SetRow(i, thisRunLineGenerator.GeneratePhotoPrintsCenters(Lxp), (i % 2) == 0);
            }
            return geoPointsGrid;
        }

        LinedGeoPoints FilterPointsGrid(const GeoPointsGrid &pointsGrid, size_t extentBorderValue) {

            if (pointsGrid.Empty())
                throw std::logic_error("GeoPoints grid is empty");
            if (pointsGrid.HasDifferentSizes())
                throw std::logic_error("GeoPoints grid has rows with different size");

            Array2D<uint8_t> containsArray(pointsGrid.Rows(), pointsGrid.Cols());
            auto initContainsItem = [this, &pointsGrid](auto iRow, auto iCol){
                return this->Contains(pointsGrid.GetGeoPoint(iRow, iCol));
            };
            containsArray.SetItems(initContainsItem);

            NearFilter<uint8_t> filter(extentBorderValue);
            auto filteredArray = filter(containsArray);

            std::vector<QList<GeoPoint>> filteredGeoPoints(pointsGrid.Rows());
            auto fillFilteredGeoPoints = [&filteredGeoPoints, &pointsGrid](auto iRow, auto iCol, uint8_t itemValue) {
                if (itemValue) {
                    if (pointsGrid.IsRowDirectOrder(iRow)) {
                        filteredGeoPoints[iRow].push_back(pointsGrid.GetGeoPoint(iRow, iCol));
                    } else {
                        filteredGeoPoints[iRow].push_front(pointsGrid.GetGeoPoint(iRow, iCol));
                    }
                }
            };
            filteredArray.IterateItems(fillFilteredGeoPoints);

            LinedGeoPoints linedGeoPoints;
            for (auto &&row : filteredGeoPoints) {
                linedGeoPoints.push_back(PlannedTrackLine(GeoPoints::fromList(row)));
            }
            return linedGeoPoints;
        }
    };

public:
    explicit AreaPhotoPrintsGenerator(const AreaPhotoRegion &area) : regionInternals_(area) {
    }

    LinedGeoPoints GeneratePhotoPrintsCenters(qreal h, qreal Lxp, qreal Lyp, qreal azimuth, size_t extentBorderValue = 0) {
        return regionInternals_.GeneratePhotoPrintsCenters(h, Lxp, Lyp, azimuth, extentBorderValue);
    }

    PhotoPrints GeneratePhotoPrints(const LinedGeoPoints &linedGeoPoints, qreal Lx, qreal Ly) {
        PhotoPrints photoPrints;
        for (auto &&linePoints : linedGeoPoints) {
            if (!linePoints.empty()) {
                LinePhotoPrintsGenerator thisRunLineGenerator(linePoints.front(), linePoints.back());
                auto thisRunLinePhotoPrints = thisRunLineGenerator.GeneratePhotoPrints(linePoints, Lx, Ly);
                photoPrints.append(thisRunLinePhotoPrints);
            }
        }
        photoPrints.squeeze();
        return photoPrints;
    }

    auto GetPreferredAzimuth() const { return regionInternals_.GetPreferredAzimuth(); }

private:
    RegionInternals regionInternals_;
};

} // aero_photo

#endif // AREAPHOTOPRINTSGENERATOR_H


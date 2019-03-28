#include "photoplanner.h"

#include "ApxApp.h"
#include "AppRoot.h"
#include "Vehicles.h"
#include "VehicleMission.h"
#include "AreaPhotoPlanner.h"
#include "Waypoint.h"
#include <iostream>

ApxPhotoplanner::ApxPhotoplanner(Fact *parent):
    Fact(parent, "photoplanner", "Photoplanner", "", Group),
    m_borderPoints(new BorderPoints()),
    m_photoPrints(new PhotoPrints()),
    m_cameraModel(20.0 / 100, 15.0 / 100, 22.5 / 100, 3648, 5472),
    m_uavModel(15, aero_photo::D2R(30))
{
    connect(ApxApp::instance(), &ApxApp::loadingFinished, this, &ApxPhotoplanner::onLoadingFinished);

    connect(m_borderPoints.get(), &BorderPoints::rowsInserted, this, &ApxPhotoplanner::onBorderPointsRowsInserted);
    connect(m_borderPoints.get(), &BorderPoints::dataChanged, this, &ApxPhotoplanner::onBorderPointsDataChanged);

    ApxApp::instance()->engine()->loadQml("qrc:/qml/PhotoplannerPlugin.qml");
}

BorderPoints *ApxPhotoplanner::getBorderPoints()
{
    return m_borderPoints.get();
}

PhotoPrints *ApxPhotoplanner::getPhotoPrints()
{
    return m_photoPrints.get();
}

void ApxPhotoplanner::onLoadingFinished()
{
    Fact *mapAdd = AppRoot::instance()->findChild("apx.tools.map.add");
    if(!mapAdd)
        return;

    m_addPhotoplannerPoint = std::make_unique<Fact>(mapAdd, "photoplannerpoint", "Photoplanner point", "");
    connect(m_addPhotoplannerPoint.get(), &Fact::triggered, this, &ApxPhotoplanner::onAddPhotoplannerPointTriggered);
    connect(m_addPhotoplannerPoint.get(), &Fact::triggered, mapAdd->parentFact(), &Fact::actionTriggered);

    m_photoplannerEdit = std::make_unique<PhotoplannerEdit>(mapAdd);
}

void ApxPhotoplanner::onAddPhotoplannerPointTriggered()
{
    Fact *fMap = AppRoot::instance()->findChild("apx.tools.map");
    QGeoCoordinate coordinate = fMap->property("clickCoordinate").value<QGeoCoordinate>();
    m_borderPoints->appendPoint(coordinate);
}

void ApxPhotoplanner::calculatePhotoPlan()
{
    float Px = 20;
    float Py = 20;
    float altitude = 100;
    float azimuth = 0;
    float velocity = 123;
    aero_photo::AreaPhotoRegion region(m_borderPoints->getAllPoints());
    try
    {
        auto planner = std::make_unique<aero_photo::AreaPhotoPlanner>(m_uavModel, m_cameraModel, region);
        if(planner->Calculate(altitude, Px, Py, azimuth, 1))
        {
            auto aeroPhotoPrints = planner->GetPhotoPrints();
            QVector<QVector<QGeoCoordinate>> photoPrints;
            auto lambda = [](auto print){return print.GetBorder();};
            std::transform(aeroPhotoPrints.begin(), aeroPhotoPrints.end(), std::back_inserter(photoPrints), lambda);
            m_photoPrints->setPrints(photoPrints);

            auto waypoints = planner->GetFlightPoints();
            VehicleMission *mission = Vehicles::instance()->current()->f_mission;
            mission->f_waypoints->f_clear->trigger();
            for(auto w: waypoints)
            {
                Waypoint *wpItem = dynamic_cast<Waypoint*>(mission->f_waypoints->addObject(w));
                if(wpItem)
                {
                    wpItem->f_altitude->setValue(w.altitude());
                    wpItem->f_type->setValue(w.type());
                    wpItem->f_actions->f_speed->setValue(velocity);
                    wpItem->f_actions->f_shot->setValue(w.shotDistance() > 0 ? "yes" : "no");
                    wpItem->f_actions->f_dshot->setValue(w.shotDistance());
                }
                else
                    apxDebug() << "Can't retrieve waypoint item";
            }
        }
        else
            apxDebug() << "Can't calculate photoplan";
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ApxPhotoplanner::onBorderPointsRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    calculatePhotoPlan();
}

void ApxPhotoplanner::onBorderPointsDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                                const QVector<int> &roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    if(roles.contains(BorderPoints::CoordinateRole))
        calculatePhotoPlan();
}

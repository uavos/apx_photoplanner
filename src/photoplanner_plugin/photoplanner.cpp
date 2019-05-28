#include "photoplanner.h"

#include "ApxApp.h"
#include "App/AppRoot.h"
#include "Vehicles/Vehicles.h"
#include "Mission/VehicleMission.h"
#include "AreaPhotoPlanner.h"
#include "LinearPhotoPlanner.h"
#include "Mission/Waypoint.h"
#include <iostream>
#include <QMessageBox>

ApxPhotoplanner::ApxPhotoplanner(Fact *parent):
    Fact(parent, "photoplanner", "Photoplanner", "", Group),
    m_borderPoints(new BorderPoints()),
    m_photoPrints(new PhotoPrints()),
    m_cameraModel(20.0 / 100, 15.0 / 100, 22.5 / 100, 3648, 5472),
    m_uavModel(15, aero_photo::D2R(30))
{
    connect(ApxApp::instance(), &ApxApp::loadingFinished, this, &ApxPhotoplanner::onLoadingFinished);

    connect(m_borderPoints.get(), &BorderPoints::rowsInserted, this, &ApxPhotoplanner::onBorderPointsRowsInserted);
    connect(m_borderPoints.get(), &BorderPoints::rowsRemoved, this, &ApxPhotoplanner::onBorderPointsRowsRemoved);
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

QString ApxPhotoplanner::getMissionType() const
{
    if(m_photoplannerEdit->getMissionType() == PhotoplannerEdit::mtArea)
        return "area";
    else if(m_photoplannerEdit->getMissionType() == PhotoplannerEdit::mtLinear)
        return "linear";
    else
        return "unknown";
}

void ApxPhotoplanner::createRemoveDialog(QGeoCoordinate coordinate)
{
    auto button = QMessageBox::question(nullptr, "Remove", "Remove this point?<br>" + coordinate.toString(),
                                        QMessageBox::Yes, QMessageBox::No);
    if(button == QMessageBox::Yes)
    {
        m_borderPoints->removePoint(coordinate);
    }
}

void ApxPhotoplanner::onLoadingFinished()
{
    Fact *mapAdd = AppRoot::instance()->findChild("apx.tools.map.add");
    if(!mapAdd)
        return;

    m_addPhotoplannerPoint = std::make_unique<Fact>(mapAdd, "photoplannerpoint", "Photoplanner point", "");
    m_addPhotoplannerPoint->setIcon("map-marker-plus");
    connect(m_addPhotoplannerPoint.get(), &Fact::triggered, this, &ApxPhotoplanner::onAddPhotoplannerPointTriggered);
    connect(m_addPhotoplannerPoint.get(), &Fact::triggered, mapAdd->parentFact(), &Fact::actionTriggered);

    m_photoplannerEdit = std::make_unique<PhotoplannerEdit>(mapAdd);
    connect(m_photoplannerEdit.get(), &PhotoplannerEdit::applyClicked, this, &ApxPhotoplanner::calculatePhotoPlan);
}

void ApxPhotoplanner::onAddPhotoplannerPointTriggered()
{
    Fact *fMap = AppRoot::instance()->findChild("apx.tools.map");
    QGeoCoordinate coordinate = fMap->property("clickCoordinate").value<QGeoCoordinate>();
    m_borderPoints->appendPoint(coordinate);
}

void ApxPhotoplanner::calculatePhotoPlan()
{
    emit missionTypeChanged();

    std::unique_ptr<aero_photo::PhotoPlanner> planner;
    auto uavModel = m_photoplannerEdit->getUavModel();
    auto cameraModel = m_photoplannerEdit->getCameraModel();
    if(m_photoplannerEdit->getMissionType() == PhotoplannerEdit::mtArea)
    {
        try
        {
            int altitude = m_photoplannerEdit->getAltitude();
            int azimuth = m_photoplannerEdit->getAzimuth();
            int px = m_photoplannerEdit->getLongitudinalOverlap();
            int py = m_photoplannerEdit->getTransverseOverlap();
            bool extentBorder = m_photoplannerEdit->getExtentAlignment();
            bool maneuverAlignment = m_photoplannerEdit->getManeuverAlignment();
            bool withPhotoPrints = m_photoplannerEdit->getWithPhotoprints();

            aero_photo::CalculationParams::Instance().enlargeEntryRequired = extentBorder;
            aero_photo::CalculationParams::Instance().maneuverAligmentRequired = maneuverAlignment;

            aero_photo::AreaPhotoRegion region(m_borderPoints->getAllPoints());
            planner = std::make_unique<aero_photo::AreaPhotoPlanner>(uavModel, cameraModel, region);
            static_cast<aero_photo::AreaPhotoPlanner*>(planner.get())->Calculate(altitude, px, py, azimuth,
                                                                                 1, withPhotoPrints);
        }
        catch(std::exception &e)
        {
            apxDebug() << e.what();
            return;
        }
    }
    else if(m_photoplannerEdit->getMissionType() == PhotoplannerEdit::mtLinear)
    {
        try
        {
            int altitude = m_photoplannerEdit->getAltitude();
            int px = m_photoplannerEdit->getLongitudinalOverlap();
            int py = m_photoplannerEdit->getTransverseOverlap();
            int width = m_photoplannerEdit->getWidth();
            bool extentBorder = m_photoplannerEdit->getExtentAlignment();
            bool maneuverAlignment = m_photoplannerEdit->getManeuverAlignment();
            bool withPhotoPrints = m_photoplannerEdit->getWithPhotoprints();

            aero_photo::CalculationParams::Instance().enlargeEntryRequired = extentBorder;
            aero_photo::CalculationParams::Instance().maneuverAligmentRequired = maneuverAlignment;

            aero_photo::LinearPhotoRegion region(m_borderPoints->getAllPoints());
            planner = std::make_unique<aero_photo::LinearPhotoPlanner>(uavModel, cameraModel, region);
            static_cast<aero_photo::LinearPhotoPlanner*>(planner.get())->Calculate(altitude, px, py, width,
                                                                                   withPhotoPrints);
        }
        catch(std::exception &e)
        {
            apxDebug() << e.what();
            return;
        }
    }

    auto aeroPhotoPrints = planner->GetPhotoPrints();
    QVector<QVector<QGeoCoordinate>> photoPrints;
    auto lambda = [](auto print){return print.GetBorder();};
    std::transform(aeroPhotoPrints.begin(), aeroPhotoPrints.end(), std::back_inserter(photoPrints), lambda);
    m_photoPrints->setPrints(photoPrints);

    int velocity = m_photoplannerEdit->getVelocity();
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

void ApxPhotoplanner::onBorderPointsRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    calculatePhotoPlan();
}

void ApxPhotoplanner::onBorderPointsRowsRemoved(const QModelIndex &parent, int first, int last)
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

#include "photoplanner.h"

#include "App/App.h"
#include "App/AppRoot.h"
#include "Vehicles/Vehicles.h"
#include "Mission/VehicleMission.h"
#include "AreaPhotoPlanner.h"
#include "LinearPhotoPlanner.h"
#include "Mission/Waypoint.h"

ApxPhotoplanner::ApxPhotoplanner(Fact *parent):
    Fact(parent, "photoplanner", "Photoplanner", "", Group, "settings"),
    m_totalDistance(0),
    m_borderPoints(new BorderPoints()),
    m_photoPrints(new PhotoPrints()),
    f_addPhotoplannerPoint(nullptr),
    f_pointEdit(nullptr),
    m_cameraModel(20.0 / 100, 15.0 / 100, 22.5 / 100, 3648, 5472),
    m_uavModel(15, qDegreesToRadians(30.0))
{
    f_cameraEdit = new CameraEdit(this);
    f_uavEdit = new UavEdit(this);
    f_planerEdit = new PlanerEdit(f_cameraEdit, this);

    connect(Vehicles::instance()->current()->f_mission, &VehicleMission::emptyChanged, this, &ApxPhotoplanner::onMissionEmptyChanged); 

    connect(App::instance(), &App::loadingFinished, this, &ApxPhotoplanner::onLoadingFinished);

    connect(m_borderPoints.get(), &BorderPoints::rowsInserted, this, &ApxPhotoplanner::onBorderPointsRowsInserted);
    connect(m_borderPoints.get(), &BorderPoints::rowsRemoved, this, &ApxPhotoplanner::onBorderPointsRowsRemoved);
    connect(m_borderPoints.get(), &BorderPoints::dataChanged, this, &ApxPhotoplanner::onBorderPointsDataChanged);

    connect(f_planerEdit, &PlanerEdit::cameraChanged, f_cameraEdit, &CameraEdit::onCurrentCameraChanged);
    connect(f_planerEdit, &PlanerEdit::uavChanged, f_uavEdit, &UavEdit::onCurrentUavChanged);

    connect(f_uavEdit->f_update, &Fact::triggered, this, &ApxPhotoplanner::calculatePhotoPlan);
    connect(f_cameraEdit->f_update, &Fact::triggered, this, &ApxPhotoplanner::calculatePhotoPlan);
    connect(f_planerEdit->f_update, &Fact::triggered, this, &ApxPhotoplanner::calculatePhotoPlan);

    App::instance()->engine()->loadQml("qrc:/qml/PhotoplannerPlugin.qml", {});
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
    if(f_planerEdit->getMissionType() == PlanerEdit::mtArea)
        return "area";
    else if(f_planerEdit->getMissionType() == PlanerEdit::mtLinear)
        return "linear";
    else
        return "unknown";
}

uint ApxPhotoplanner::getTotalDistance() const
{
    return m_totalDistance;
}

void ApxPhotoplanner::createEditor(int id, QGeoCoordinate coordinate)
{
    f_pointEdit = new PointEdit(nullptr, id, coordinate);
    f_pointEdit->setIcon("settings");
    connect(f_pointEdit, &PointEdit::removed, this, [=]() { f_pointEdit = nullptr; });
    connect(f_pointEdit, &PointEdit::updatePointTriggered, m_borderPoints.get(), &BorderPoints::updatePoint);
    connect(f_pointEdit, &PointEdit::removePointTriggered, m_borderPoints.get(), &BorderPoints::removePoint);
    f_pointEdit->trigger();
    App::jsync(this);
}

void ApxPhotoplanner::onLoadingFinished()
{
    Fact *mapAdd = AppRoot::instance()->findChild("apx.tools.missionplanner.add");
    if(!mapAdd)
        return;

    if(f_addPhotoplannerPoint)
        delete f_addPhotoplannerPoint;
    f_addPhotoplannerPoint = new Fact(mapAdd, "photoplannerpoint", "Photoplanner point", "", Fact::CloseOnTrigger);
    f_addPhotoplannerPoint->setIcon("map-marker-plus");
    connect(f_addPhotoplannerPoint, &Fact::triggered, this, &ApxPhotoplanner::onAddPhotoplannerPointTriggered);
    connect(f_addPhotoplannerPoint, &Fact::triggered, mapAdd->parentFact(), &Fact::triggered);
}

void ApxPhotoplanner::onAddPhotoplannerPointTriggered()
{
    Fact *fMap = AppRoot::instance()->findChild("apx.tools.missionplanner");
    QGeoCoordinate coordinate = fMap->property("clickCoordinate").value<QGeoCoordinate>();
    m_borderPoints->appendPoint(coordinate);
}

void ApxPhotoplanner::calculatePhotoPlan()
{
    emit missionTypeChanged();

    std::unique_ptr<aero_photo::PhotoPlanner> planner;
    auto uavModel = f_uavEdit->getUavModel();
    auto cameraModel = f_cameraEdit->getCameraModel();
    auto mission = Vehicles::instance()->current()->f_mission;
    if(f_planerEdit->getMissionType() == PlanerEdit::mtArea) {
        try {
            int altitude = f_planerEdit->getAltitude();
            int azimuth = f_planerEdit->getAzimuth();
            int px = f_planerEdit->getLongitudinalOverlap();
            int py = f_planerEdit->getTransverseOverlap();
            bool extentBorder = f_planerEdit->getExtentAlignment();
            bool maneuverAlignment = f_planerEdit->getManeuverAlignment();
            bool withPhotoPrints = f_planerEdit->getWithPhotoprints();

            aero_photo::CalculationParams::Instance().enlargeEntryRequired = extentBorder;
            aero_photo::CalculationParams::Instance().maneuverAligmentRequired = maneuverAlignment;

            aero_photo::AreaPhotoRegion region(m_borderPoints->getAllPoints());
            planner = std::make_unique<aero_photo::AreaPhotoPlanner>(uavModel, cameraModel, region);
            static_cast<aero_photo::AreaPhotoPlanner *>(planner.get())->Calculate(altitude, px, py, azimuth, 1, withPhotoPrints);
        } catch(std::exception &e) {
            apxDebug() << e.what();
            mission->f_waypoints->f_clear->trigger();
            return;
        }
    } else if(f_planerEdit->getMissionType() == PlanerEdit::mtLinear) {
        try {
            int altitude = f_planerEdit->getAltitude();
            int px = f_planerEdit->getLongitudinalOverlap();
            int py = f_planerEdit->getTransverseOverlap();
            int width = f_planerEdit->getWidth();
            bool extentBorder = f_planerEdit->getExtentAlignment();
            bool maneuverAlignment = f_planerEdit->getManeuverAlignment();
            bool withPhotoPrints = f_planerEdit->getWithPhotoprints();

            aero_photo::CalculationParams::Instance().enlargeEntryRequired = extentBorder;
            aero_photo::CalculationParams::Instance().maneuverAligmentRequired = maneuverAlignment;

            aero_photo::LinearPhotoRegion region(m_borderPoints->getAllPoints());
            planner = std::make_unique<aero_photo::LinearPhotoPlanner>(uavModel, cameraModel, region);
            static_cast<aero_photo::LinearPhotoPlanner *>(planner.get())->Calculate(altitude, px, py, width, withPhotoPrints);
        } catch(std::exception &e) {
            apxDebug() << e.what();
            mission->f_waypoints->f_clear->trigger();
            return;
        }
    }

    auto aeroPhotoPrints = planner->GetPhotoPrints();
    QVector<QVector<QGeoCoordinate>> photoPrints;
    auto lambda = [](auto print) { return print.GetBorder(); };
    std::transform(aeroPhotoPrints.begin(), aeroPhotoPrints.end(), std::back_inserter(photoPrints), lambda);
    m_photoPrints->setPrints(photoPrints);

    bool useSpeedInWaypoint = f_uavEdit->getUseSpeedInWaypoint();
    int velocity = f_uavEdit->getVelocity();
    auto waypoints = planner->GetFlightPoints();
    mission->f_waypoints->f_clear->trigger();
    for(auto w: waypoints) {
        Waypoint *wpItem = dynamic_cast<Waypoint *>(mission->f_waypoints->addObject(w));
        if(wpItem) {
            wpItem->f_altitude->setValue(w.altitude());
            wpItem->f_type->setValue(w.type());
            if(useSpeedInWaypoint)
                wpItem->f_actions->f_speed->setValue(velocity);
            wpItem->f_actions->f_shot->setValue(w.shotDistance() > 0 ? "single" : "no");
            wpItem->f_actions->f_dshot->setValue(w.shotDistance());
        } else
            apxDebug() << "Can't retrieve waypoint item";
    }
    m_totalDistance = mission->f_waypoints->distance();
    emit totalDistanceChanged();
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

void ApxPhotoplanner::onMissionEmptyChanged()
{
    if(Vehicles::instance()->current()->f_mission->empty()) {
        m_borderPoints->clear();
        m_photoPrints->setPrints({});
    }
}
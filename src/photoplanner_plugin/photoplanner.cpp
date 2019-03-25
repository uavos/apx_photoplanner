#include "photoplanner.h"

#include "ApxApp.h"
#include "AppRoot.h"
#include "AreaPhotoPlanner.h"
#include <iostream>

ApxPhotoplanner::ApxPhotoplanner(Fact *parent):
    Fact(parent, "photoplanner", "Photoplanner", "", Group),
    m_points(new Points()),
    m_cameraModel(20.0 / 100, 15.0 / 100, 22.5 / 100, 3648, 5472),
    m_uavModel(15, aero_photo::D2R(30))
{
    connect(ApxApp::instance(), &ApxApp::loadingFinished, this, &ApxPhotoplanner::onLoadingFinished);

    connect(m_points.get(), &Points::rowsInserted, this, &ApxPhotoplanner::onPointsRowsInserted);

    ApxApp::instance()->engine()->loadQml("qrc:/qml/PhotoplannerPlugin.qml");
}

Points *ApxPhotoplanner::getPoints()
{
    return m_points.get();
}

void ApxPhotoplanner::onLoadingFinished()
{
    Fact *mapAdd = AppRoot::instance()->findChild("apx.tools.map.add");
    if(!mapAdd)
        return;

    m_addPhotoplannerPoint = std::make_unique<Fact>(mapAdd, "photoplannerpoint", "Photoplanner point", "");
    connect(m_addPhotoplannerPoint.get(), &Fact::triggered, this, &ApxPhotoplanner::onAddPhotoplannerPointTriggered);
    connect(m_addPhotoplannerPoint.get(), &Fact::triggered, mapAdd->parentFact(), &Fact::actionTriggered);

    //    PhotoplanEdit *f = new PhotoplanEdit(mapAdd);
}

void ApxPhotoplanner::onAddPhotoplannerPointTriggered()
{
    Fact *fMap = AppRoot::instance()->findChild("apx.tools.map");
    QGeoCoordinate coordinate = fMap->property("clickCoordinate").value<QGeoCoordinate>();
    m_points->appendPoint(coordinate);
}

void ApxPhotoplanner::onPointsRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    std::cout << "INSERTED" << std::endl;

    float Px = 20;
    float Py = 20;
    float altitude = 100;
    float azimuth = 0;
    aero_photo::AreaPhotoRegion region(m_points->getAllPoints());
    try
    {
        auto planner = std::make_unique<aero_photo::AreaPhotoPlanner>(m_uavModel, m_cameraModel, region);
        std::cout << "CALCULATED: " << planner->Calculate(altitude, Px, Py, azimuth, 1) << std::endl;
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

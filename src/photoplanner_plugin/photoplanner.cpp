#include "photoplanner.h"

#include "ApxApp.h"
#include "AppRoot.h"
#include "AreaPhotoPlanner.h"
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

    //    PhotoplanEdit *f = new PhotoplanEdit(mapAdd);
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
    aero_photo::AreaPhotoRegion region(m_borderPoints->getAllPoints());
    try
    {
        auto planner = std::make_unique<aero_photo::AreaPhotoPlanner>(m_uavModel, m_cameraModel, region);
        if(planner->Calculate(altitude, Px, Py, azimuth, 1))
        {
            auto aeroPhotoPrints = planner->GetPhotoPrints();
            QVector<QVector<QGeoCoordinate>> photoPrints;
            std::transform(aeroPhotoPrints.begin(), aeroPhotoPrints.end(), std::back_inserter(photoPrints), [](auto print){
                return print.GetBorder();
            });
            m_photoPrints->setPrints(photoPrints);
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

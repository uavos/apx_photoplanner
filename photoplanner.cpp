#include "photoplanner.h"

#include "ApxApp.h"
#include "AppRoot.h"
#include "photoplanedit.h"
#include <iostream>

Photoplanner::Photoplanner(Fact *parent):
    Fact(parent, "photoplanner", "Photoplanner", "", Group),
    m_points(new Points())
{
    connect(ApxApp::instance(), &ApxApp::loadingFinished, this, &Photoplanner::onLoadingFinished);

    ApxApp::instance()->engine()->loadQml("qrc:/qml/PhotoplannerPlugin.qml");
}

Points *Photoplanner::getPoints()
{
    return m_points.get();
}

void Photoplanner::onLoadingFinished()
{
    Fact *mapAdd = AppRoot::instance()->findChild("apx.tools.map.add");
    if(!mapAdd)
        return;

    m_addPhotoplannerPoint = std::make_unique<Fact>(mapAdd, "photoplannerpoint", "Photoplanner point", "");
    connect(m_addPhotoplannerPoint.get(), &Fact::triggered, this, &Photoplanner::onAddPhotoplannerPointTriggered);
    connect(m_addPhotoplannerPoint.get(), &Fact::triggered, mapAdd->parentFact(), &Fact::actionTriggered);

//    PhotoplanEdit *f = new PhotoplanEdit(mapAdd);
}

void Photoplanner::onAddPhotoplannerPointTriggered()
{
    Fact *fMap = AppRoot::instance()->findChild("apx.tools.map");
    QGeoCoordinate coordinate = fMap->property("clickCoordinate").value<QGeoCoordinate>();
    m_points->appendPoint(coordinate);
}

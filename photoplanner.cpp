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

    m_points->appendPoint(QGeoCoordinate(37.406367, -122.046297));
    m_points->appendPoint(QGeoCoordinate(37.406583, -122.041839));
}

Points *Photoplanner::getPoints()
{
    return m_points;
}

void Photoplanner::onLoadingFinished()
{
    Fact *fMapAdd = AppRoot::instance()->findChild("apx.tools.map.add");
    if (!fMapAdd)
        return;
    //create tool for map
    PhotoplanEdit *f = new PhotoplanEdit(fMapAdd);
//    f->setIcon("city");
//    connect(f, &SiteEdit::addTriggered, f_add, &SiteEdit::addTriggered);
}

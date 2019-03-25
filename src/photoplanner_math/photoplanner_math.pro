#-------------------------------------------------
#
# Project created by QtCreator 2017-07-03T21:08:48
#
#-------------------------------------------------

QT       += positioning

TARGET = photoplanner_math
TEMPLATE = lib
CONFIG += staticlib qt exception c++14 silent

DESTDIR = ../../bin
OBJECTS_DIR = ../../obj/math
UI_DIR = $$OBJECTS_DIR
MOC_DIR = $$OBJECTS_DIR

HEADERS += \
    AreaPhotoPlanner.h \
    AreaPhotoPrintsGenerator.h \
    Array2d.h \
    Azimuth.h \
    LinearPhotoPlanner.h \
    LinearPhotoPrintsGenerator.h \
    LinedGeoPoints.h \
    LinePhotoPrintsGenerator.h \
    ManeuverTrackAlignment.h \
    NearFilter.h \
    PhotoCameraModel.h \
    PhotoGeoCalcs.h \
    PhotoPlanner.h \
    PhotoPlannerCore.h \
    PhotoPrintsGenerators.h \
    PhotoUavModel.h \
    PhotoPlannerXmlWriter.h

SOURCES += \
    PhotoPlannerCore.cpp

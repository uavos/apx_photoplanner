#-------------------------------------------------
#
# Project created by QtCreator 2019-03-20T14:09:35
#
#-------------------------------------------------

QT += quick widgets positioning

TARGET = apx_photoplanner
TEMPLATE = lib

CONFIG += plugin c++14 silent

DESTDIR = ../../bin
OBJECTS_DIR = ../../obj/plugin
UI_DIR = $$OBJECTS_DIR
MOC_DIR = $$OBJECTS_DIR
target.path = ~/Документы/UAVOS/Plugins/
INSTALLS += target

SOURCES += \
    photoplannerplugin.cpp \
    photoplanner.cpp \
    borderpoints.cpp \
    photoprints.cpp \
    photoplanneredit.cpp \
    cameraanduavdata.cpp

HEADERS += \
    photoplannerplugin.h \
    photoplanner.h \
    borderpoints.h \
    photoprints.h \
    photoplanneredit.h \
    cameraanduavdata.h

INCLUDEPATH += /home/pavel/sources/build-apx-release/gcs/release/install-root/usr/lib/gcs/include/
LIBS += -L/home/pavel/sources/build-apx-release/gcs/release/install-root/usr/lib/gcs/ \
    -lApxCore \
    -lApxData

INCLUDEPATH += ../photoplanner_math
LIBS += -L../../bin \
    -lphotoplanner_math

RESOURCES += \
    stuff.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2019-03-20T14:09:35
#
#-------------------------------------------------

QT += quick widgets positioning

TARGET = apx_photoplanner
TEMPLATE = lib

CONFIG += plugin c++14

DESTDIR = bin
OBJECTS_DIR = obj
UI_DIR = obj
MOC_DIR = obj
target.path = ~/Документы/UAVOS/Plugins/
INSTALLS += target

SOURCES += \
    photoplannerplugin.cpp \
    photoplanner.cpp \
    photoplanedit.cpp \
    points.cpp

HEADERS += \
    photoplannerplugin.h \
    photoplanner.h \
    photoplanedit.h \
    points.h

INCLUDEPATH += /home/pavel/sources/build-apx-release/gcs/release/install-root/usr/lib/gcs/include/
LIBS += -L/home/pavel/sources/build-apx-release/gcs/release/install-root/usr/lib/gcs/ \
    -lApxCore \
    -lApxData

RESOURCES += \
    stuff.qrc

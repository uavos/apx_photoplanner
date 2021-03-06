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
    cameraedit.cpp \
    photoplannerplugin.cpp \
    photoplanner.cpp \
    borderpoints.cpp \
    photoprints.cpp \
    planeredit.cpp \
    pointedit.cpp \
    uavedit.cpp

HEADERS += \
    cameraedit.h \
    photoplannerplugin.h \
    photoplanner.h \
    borderpoints.h \
    photoprints.h \
    planeredit.h \
    pointedit.h \
    uavedit.h

INCLUDEPATH += /home/pavel/apx_sdk/include/ApxGcs
INCLUDEPATH += /home/pavel/apx_sdk/include/ApxCore
INCLUDEPATH += /home/pavel/apx_sdk/include/ApxData
INCLUDEPATH += /home/pavel/apx_sdk/include/ApxShared
LIBS += -L/home/pavel/apx_sdk/lib \
    -lApxCore \
    -lApxData \
    -lApxGcs \
    -lApxShared

INCLUDEPATH += ../photoplanner_math
LIBS += -L../../bin \
    -lphotoplanner_math

RESOURCES += \
    stuff.qrc

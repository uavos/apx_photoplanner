#ifndef PHOTOPLANNEREDIT_H
#define PHOTOPLANNEREDIT_H

#include <memory>
#include "Fact.h"
#include "PhotoCameraModel.h"
#include "PhotoUavModel.h"
#include "cameraanduavdata.h"

class PhotoplannerEdit: public Fact
{
    Q_OBJECT
public:
    enum MissionType
    {
        mtArea = 0,
        mtLinear = 1
    };
    PhotoplannerEdit(Fact *parent);

private:
    std::unique_ptr<Fact> m_plannerParams;
    std::unique_ptr<Fact> m_cameraParams;
    std::unique_ptr<Fact> m_uavParams;
    //planer
    std::unique_ptr<Fact> m_missionType;
    std::unique_ptr<Fact> m_uavModel;
    std::unique_ptr<Fact> m_cameraModel;
    std::unique_ptr<Fact> m_extendAlignment;
    std::unique_ptr<Fact> m_maneuverAlignment;
    std::unique_ptr<Fact> m_longitudinalOverlap;
    std::unique_ptr<Fact> m_transverseOverlap;
    std::unique_ptr<Fact> m_altitude;
    std::unique_ptr<Fact> m_gsd;
    std::unique_ptr<Fact> m_azimuth;
    std::unique_ptr<Fact> m_width;
    std::unique_ptr<Fact> m_runs;
    std::unique_ptr<Fact> m_reverseDirection;
    //camera
    std::unique_ptr<Fact> m_cameraName;
    std::unique_ptr<Fact> m_focusRange;
    std::unique_ptr<Fact> m_sensorLx;
    std::unique_ptr<Fact> m_sensorLy;
    std::unique_ptr<Fact> m_sensorAx;
    std::unique_ptr<Fact> m_sensorAy;
    //uav
    std::unique_ptr<Fact> m_uavName;
    std::unique_ptr<Fact> m_flightTime;
    std::unique_ptr<Fact> m_flightSpeed;
    std::unique_ptr<Fact> m_commRadius;
    std::unique_ptr<Fact> m_maxRoll;
    std::unique_ptr<Fact> m_maneuverR;

    int calcWidth();
    int calcRuns();
    int calcGsd();
    int calcAltitude();
    int calcUavRoll();
    int calcUavR();
    void calcWidthAndRuns();
    void calcAltitudeAndGsd();
    void calcUavRAndRoll();
    aero_photo::PhotoUavModel createUavModel();
    aero_photo::PhotoCameraModel createCameraModel();
    CameraData createCameraDataFromGui();
    UavData createUavDataFromGui();

    void saveCameraData();
    void loadCameraData(int idx);
    void saveUavData();
    void loadUavData(int idx);
    QStringList getUavNames();
    QStringList getCameraNames();

    void writeDefaultUavAndCameraData();

    void setValueLikeANinja(FactData *data, const QVariant &value); //prevent cyclic setValue->valueChanged calling
    bool m_ninja;

private slots:
    void onMissionTypeValueChanged();
    void onLongitudinalOverlapValueChanged();
    void onTransverseOverlapValueChanged();
    void onAltitudeValueChanged();
    void onGsdValueChanged();
    void onWidthValueChanged();
    void onRunsChanged();
    void onMaxRollValueChanged();
    void onManeuverRValueChanged();
    void onFlightSpeedValueChanged();
    void onUavModelValueChanged();
    void onCameraModelValueChanged();
    void onUavAnyParamChanged();
    void onCameraAnyParamChanged();
};

#endif // PHOTOPLANNEREDIT_H

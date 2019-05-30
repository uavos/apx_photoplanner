#ifndef PHOTOPLANNEREDIT_H
#define PHOTOPLANNEREDIT_H

#include <memory>
#include "Fact/Fact.h"
#include "PhotoCameraModel.h"
#include "PhotoUavModel.h"

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

    aero_photo::PhotoUavModel getUavModel();
    aero_photo::PhotoCameraModel getCameraModel();
    int getMissionType();
    int getAltitude() const;
    int getAzimuth() const;
    int getLongitudinalOverlap() const;
    int getTransverseOverlap() const;
    bool getExtentAlignment() const;
    bool getManeuverAlignment() const;
    int getWidth() const;
    int getVelocity() const;
    bool getUseSpeedInWaypoint() const;
    bool getWithPhotoprints() const;

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
    std::unique_ptr<Fact> m_withPhotoprints;
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
    std::unique_ptr<Fact> m_useSpeedInWaypoint;
    std::unique_ptr<Fact> m_commRadius;
    std::unique_ptr<Fact> m_maxRoll;
    std::unique_ptr<Fact> m_maneuverR;
    //
    std::unique_ptr<FactAction> m_applyButton;

    int calcWidth();
    int calcRuns();
    int calcGsd();
    int calcAltitude();
    int calcUavRoll();
    int calcUavR();
    void calcWidthAndRuns();
    void calcAltitudeAndGsd();
    void calcUavRAndRoll();

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

signals:
    void applyClicked();
};

#endif // PHOTOPLANNEREDIT_H

#pragma once

#include "Fact/Fact.h"
#include "cameraedit.h"

class PlanerEdit: public Fact
{
    Q_OBJECT
public:
    enum MissionType
    {
        mtArea = 0,
        mtLinear = 1
    };
    PlanerEdit(CameraEdit *camera, Fact *parent = nullptr);

    Fact *f_missionType;
    Fact *f_uavModel;
    Fact *f_cameraModel;
    Fact *f_extendAlignment;
    Fact *f_maneuverAlignment;
    Fact *f_longitudinalOverlap;
    Fact *f_transverseOverlap;
    Fact *f_altitude;
    Fact *f_gsd;
    Fact *f_azimuth;
    Fact *f_width;
    Fact *f_runs;
    Fact *f_reverseDirection;
    Fact *f_withPhotoprints;
    Fact *f_update;

    int getMissionType();
    int getAltitude() const;
    int getAzimuth() const;
    int getLongitudinalOverlap() const;
    int getTransverseOverlap() const;
    bool getExtentAlignment() const;
    bool getManeuverAlignment() const;
    int getWidth() const;
    bool getWithPhotoprints() const;

private:
    CameraEdit *m_cameraEdit;
    bool m_ninja = false;
    int calcWidth();
    void calcWidthAndRuns();
    int calcRuns();
    int calcGsd();
    int calcAltitude();
    void calcAltitudeAndGsd();
    void setValueLikeANinja(FactData *data, const QVariant &value);
    QStringList getUavNames();
    QStringList getCameraNames();

private slots:
    void onMissionTypeValueChanged();
    void onLongitudinalOverlapValueChanged();
    void onTransverseOverlapValueChanged();
    void onAltitudeValueChanged();
    void onGsdValueChanged();
    void onWidthValueChanged();
    void onRunsChanged();
    void onUavModelValueChanged();
    void onCameraModelValueChanged();

signals:
    void uavChanged(int index);
    void cameraChanged(int index);
};

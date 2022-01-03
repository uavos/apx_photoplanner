#pragma once

#include "Fact/Fact.h"
#include "PhotoUavModel.h"

class UavEdit: public Fact
{
    Q_OBJECT
public:
    UavEdit(Fact *parent = nullptr);

    int getVelocity() const;
    bool getUseSpeedInWaypoint() const;

    aero_photo::PhotoUavModel getUavModel();

    Fact *f_uavName;
    Fact *f_flightTime;
    Fact *f_flightSpeed;
    Fact *f_useSpeedInWaypoint;
    Fact *f_commRadius;
    Fact *f_maxRoll;
    Fact *f_maneuverR;
    Fact *f_update;

public slots:
    void onCurrentUavChanged(int index);

private:
    int m_index = 0;

    bool m_ninja = false;

    void calcUavRAndRoll();
    int calcUavRoll();
    int calcUavR();

    void loadData();

private:
    void setValueLikeANinja(FactData *data, const QVariant &value);
    void saveUavData();
    void writeDefaultUavData();

private slots:
    void onMaxRollValueChanged();
    void onManeuverRValueChanged();
    void onFlightSpeedValueChanged();
};

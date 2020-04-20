#ifndef CAMERAEDIT_H
#define CAMERAEDIT_H

#include "Fact/Fact.h"
#include "PhotoCameraModel.h"

class CameraEdit: public Fact
{
    Q_OBJECT
public:
    CameraEdit(Fact *parent = nullptr);

    aero_photo::PhotoCameraModel getCameraModel();

    Fact *f_cameraName;
    Fact *f_focusRange;
    Fact *f_sensorLx;
    Fact *f_sensorLy;
    Fact *f_sensorAx;
    Fact *f_sensorAy;
    Fact *f_update;

public slots:
    void onCurrentCameraChanged(int index);

private:
    int m_index = 0;

    void saveCameraData();
    void writeDefaultCameraData();

    void loadData();
};

#endif // CAMERAEDIT_H

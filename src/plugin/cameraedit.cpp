#include "cameraedit.h"

#include "App/AppPrefs.h"

CameraEdit::CameraEdit(Fact *parent):
    Fact(parent, "camera", "Camera", "", Section, "camera")
{
    f_cameraName = new Fact(this, "camera_name", "Name", "", Text, "camera");
    f_focusRange = new Fact(this, "focus_range", "Focus range", "", Float, "image-filter-center-focus");
    f_sensorLx = new Fact(this, "sensor_lx", "Sensor lx", "", Float, "camera-metering-matrix");
    f_sensorLy = new Fact(this, "sensor_ly", "Sensor ly", "", Float, "camera-metering-matrix");
    f_sensorAx = new Fact(this, "sensor_ax", "Sensor ax", "", Float, "camera-metering-spot");
    f_sensorAy = new Fact(this, "sensor_ay", "Sensor ay", "", Float, "camera-metering-spot");
    f_update = new Fact(this, "update", "Update", "", Fact::Action | Fact::Apply | Fact::CloseOnTrigger);

    f_focusRange->setMin(1);
    f_focusRange->setMax(100);
    f_focusRange->setUnits("mm");

    f_sensorLx->setMin(1);
    f_sensorLx->setMax(100);
    f_sensorLx->setUnits("mm");

    f_sensorLy->setMin(1);
    f_sensorLy->setMax(100);
    f_sensorLy->setUnits("mm");

    f_sensorAx->setMin(500);
    f_sensorAx->setMax(10000);
    f_sensorAx->setUnits("px");

    f_sensorAy->setMin(500);
    f_sensorAy->setMax(10000);
    f_sensorAy->setUnits("px");

    writeDefaultCameraData();

    loadData();

    for(int i = 0; i < size(); i++)
        connect(child(i), &Fact::valueChanged, this, &CameraEdit::saveCameraData);
}

aero_photo::PhotoCameraModel CameraEdit::getCameraModel()
{
    aero_photo::PhotoCameraModel model(f_focusRange->value().toFloat() / 100.0,
                                       f_sensorLx->value().toFloat() / 100.0,
                                       f_sensorLy->value().toFloat() / 100.0,
                                       f_sensorAx->value().toFloat(),
                                       f_sensorAy->value().toFloat());
    return model;
}

void CameraEdit::onCurrentCameraChanged(int index)
{
    m_index = index;
    loadData();
}

void CameraEdit::saveCameraData()
{
    QSettings *settings = AppPrefs::settings();
    settings->beginGroup("photoplanner");
    QString str = QJsonDocument(valuesToJson()).toJson(QJsonDocument::Compact);
    settings->setValue(QString("camera_%1").arg(m_index), str);
    settings->endGroup();
}

void CameraEdit::writeDefaultCameraData()
{
    QList<QJsonObject> cameraData = {
        {{"camera_name", "Sony A6000 [Sel20F28]"}, {"focus_range", 20}, {"sensor_lx", 15}, {"sensor_ly", 22.5}, {"sensor_ax", 3648}, {"sensor_ay", 5472}},
        {{"camera_name", "Sony A6000 [Sel35F18]"}, {"focus_range", 35}, {"sensor_lx", 15}, {"sensor_ly", 22.5}, {"sensor_ax", 3648}, {"sensor_ay", 5472}},
        {{"camera_name", "Sony S600 35"}, {"focus_range", 35}, {"sensor_lx", 15}, {"sensor_ly", 22.5}, {"sensor_ax", 2112}, {"sensor_ay", 2816}},
        {{"camera_name", "Sony S600 50"}, {"focus_range", 50}, {"sensor_lx", 15}, {"sensor_ly", 22.5}, {"sensor_ax", 2112}, {"sensor_ay", 2816}},
        {{"camera_name", "User camera 1"}, {"focus_range", 50}, {"sensor_lx", 15}, {"sensor_ly", 22.5}, {"sensor_ax", 2112}, {"sensor_ay", 2816}},
        {{"camera_name", "User camera 2"}, {"focus_range", 50}, {"sensor_lx", 15}, {"sensor_ly", 22.5}, {"sensor_ax", 2112}, {"sensor_ay", 2816}},
        {{"camera_name", "User camera 3"}, {"focus_range", 50}, {"sensor_lx", 15}, {"sensor_ly", 22.5}, {"sensor_ax", 2112}, {"sensor_ay", 2816}}
    };

    QSettings *settings = AppPrefs::settings();
    settings->beginGroup("photoplanner");
    for(int i = 0; i < cameraData.size(); i++)
    {
        QString key = QString("camera_%1").arg(i);
        QString str = settings->value(key).toString();
        if(str.isEmpty())
        {
            str = QJsonDocument(cameraData[i]).toJson(QJsonDocument::Compact);
            settings->setValue(key, str);
        }
    }
    settings->endGroup();
    settings->sync();
}

void CameraEdit::loadData()
{
    auto settings = AppPrefs::instance()->settings();
    settings->beginGroup("photoplanner");
    auto str = settings->value(QString("camera_%1").arg(m_index)).toString();
    auto j = QJsonDocument::fromJson(str.toUtf8()).object();
    valuesFromJson(j);
    settings->endGroup();
}

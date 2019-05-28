#include "photoplanneredit.h"

#include <QSettings>
#include "ApxLog.h"
#include "ApxApp.h"
#include "PhotoCameraModel.h"
#include "PhotoGeoCalcs.h"
#include "App/AppSettings.h"

PhotoplannerEdit::PhotoplannerEdit(Fact *parent):
    Fact(parent, "photoplanner_edit", "Photoplanner", "Photoplanenr parameters", Group),
    m_plannerParams(new Fact(this, "planner", "Planner", "", Section)),
    m_cameraParams(new Fact(this, "camera", "Camera", "", Section)),
    m_uavParams(new Fact(this, "uav", "Uav", "", Section)),
    //planer
    m_missionType(new Fact(m_plannerParams.get(), "mission_type", "Mission type", "", Enum)),
    m_uavModel(new Fact(m_plannerParams.get(), "uav_model", "Uav model", "", Enum)),
    m_cameraModel(new Fact(m_plannerParams.get(), "camera_model", "Camera model", "", Enum)),
    m_extendAlignment(new Fact(m_plannerParams.get(), "extend_alignment", "Extend alignment", "", Bool)),
    m_maneuverAlignment(new Fact(m_plannerParams.get(), "maneuver_alignment", "Use maneuver for alignment", "", Bool)),
    m_longitudinalOverlap(new Fact(m_plannerParams.get(), "longitudinal_overlap", "Longitudinal overlap", "", Int)),
    m_transverseOverlap(new Fact(m_plannerParams.get(), "transverse_overlap", "Transverse overlap", "", Int)),
    m_altitude(new Fact(m_plannerParams.get(), "altitude", "Altitude", "", Int)),
    m_gsd(new Fact(m_plannerParams.get(), "gsd", "GSD", "", Int)),
    m_azimuth(new Fact(m_plannerParams.get(), "azimuth", "Azimuth", "", Int)),
    m_width(new Fact(m_plannerParams.get(), "width", "Width", "", Int)),
    m_runs(new Fact(m_plannerParams.get(), "runs", "Runs", "", Int)),
    m_reverseDirection(new Fact(m_plannerParams.get(), "reverse_direction", "Reverse direction", "", Bool)),
    m_withPhotoprints(new Fact(m_plannerParams.get(), "with_photoprints", "With photoprints", "", Bool)),
    //camera
    m_cameraName(new Fact(m_cameraParams.get(), "camera_name", "Name", "", Text)),
    m_focusRange(new Fact(m_cameraParams.get(), "focus_range", "Focus range", "", Int)),
    m_sensorLx(new Fact(m_cameraParams.get(), "sensor_lx", "Sensor lx", "", Float)),
    m_sensorLy(new Fact(m_cameraParams.get(), "sensor_ly", "Sensor ly", "", Float)),
    m_sensorAx(new Fact(m_cameraParams.get(), "sensor_ax", "Sensor ax", "", Int)),
    m_sensorAy(new Fact(m_cameraParams.get(), "sensor_ay", "Sensor ay", "", Int)),
    //uav
    m_uavName(new Fact(m_uavParams.get(), "uav_name", "Name", "", Text)),
    m_flightTime(new Fact(m_uavParams.get(), "flight_time", "Flight time", "", Int)),
    m_flightSpeed(new Fact(m_uavParams.get(), "flight_speed", "Flight speed", "", Int)),
    m_commRadius(new Fact(m_uavParams.get(), "comm_radius", "Comm radius", "", Int)),
    m_maxRoll(new Fact(m_uavParams.get(), "max_roll", "Max roll", "", Int)),
    m_maneuverR(new Fact(m_uavParams.get(), "maneuver_r", "Maneuver R", "", Int)),
    //
    m_applyButton(new FactAction(this, "update", "Update", "", "", FactAction::ActionApply)),
    m_ninja(false)
{
    m_missionType->setIcon("map");
    m_cameraModel->setIcon("camera");
    m_uavModel->setIcon("airplane");
    m_cameraName->setIcon("camera");
    m_uavName->setIcon("airplane");
    m_flightSpeed->setIcon("speedometer");
    m_flightTime->setIcon("clock");
    m_commRadius->setIcon("vector-radius");
    m_azimuth->setIcon("compass");
    m_focusRange->setIcon("image-filter-center-focus");
    m_altitude->setIcon("airplane-takeoff");
    m_extendAlignment->setIcon("shape-polygon-plus");
    m_sensorLx->setIcon("camera-metering-matrix");
    m_sensorLy->setIcon("camera-metering-matrix");
    m_sensorAx->setIcon("camera-metering-spot");
    m_sensorAy->setIcon("camera-metering-spot");
    m_longitudinalOverlap->setIcon("vector-intersection");
    m_transverseOverlap->setIcon("vector-intersection");
    m_gsd->setIcon("details");
    m_maneuverR->setIcon("radius");
    m_maxRoll->setIcon("rotate-right");
    m_maneuverAlignment->setIcon("call-missed");
    m_maneuverR->setIcon("map-marker-distance");
    m_withPhotoprints->setIcon("image-multiple");

    setIcon("settings");
    model()->setFlat(true);

    m_missionType->setEnumStrings({"area", "linear"}, {mtArea, mtLinear});

    m_extendAlignment->setValue(true);
    m_maneuverAlignment->setValue(true);

    m_longitudinalOverlap->setMin(0);
    m_longitudinalOverlap->setMax(95);
    m_longitudinalOverlap->setUnits("%");

    m_transverseOverlap->setMin(0);
    m_transverseOverlap->setMax(95);
    m_transverseOverlap->setUnits("%");

    m_altitude->setMin(5);
    m_altitude->setMax(5000);
    m_altitude->setUnits("m");

    m_gsd->setMin(1);
    m_gsd->setMax(1000);
    m_gsd->setUnits("sm/pix");

    m_azimuth->setMin(0);
    m_azimuth->setMax(359);
    m_azimuth->setUnits("deg");

    m_width->setMin(1);
    m_width->setMax(1000);
    m_width->setUnits("m");

    m_runs->setMin(1);
    m_runs->setMax(10);

    m_withPhotoprints->setValue(false);

    m_focusRange->setMin(1);
    m_focusRange->setMax(100);
    m_focusRange->setUnits("mm");

    m_sensorLx->setMin(1);
    m_sensorLx->setMax(100);
    m_sensorLx->setUnits("mm");

    m_sensorLy->setMin(1);
    m_sensorLy->setMax(100);
    m_sensorLy->setUnits("mm");

    m_sensorAx->setMin(500);
    m_sensorAx->setMax(10000);
    m_sensorAx->setUnits("px");

    m_sensorAy->setMin(500);
    m_sensorAy->setMax(10000);
    m_sensorAy->setUnits("px");

    m_flightTime->setMin(1);
    m_flightTime->setMax(240);
    m_flightTime->setUnits("min");

    m_flightSpeed->setMin(1);
    m_flightSpeed->setMax(200);
    m_flightSpeed->setUnits("m/s");

    m_commRadius->setMin(1);
    m_commRadius->setMax(120);
    m_commRadius->setUnits("km");

    m_maxRoll->setMin(1);
    m_maxRoll->setMax(90);
    m_maxRoll->setUnits("deg");

    m_maneuverR->setMin(0);
    m_maneuverR->setMax(5000);
    m_maneuverR->setUnits("m");

    connect(m_missionType.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onMissionTypeValueChanged);
    connect(m_longitudinalOverlap.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onLongitudinalOverlapValueChanged);
    connect(m_transverseOverlap.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onTransverseOverlapValueChanged);
    connect(m_altitude.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onAltitudeValueChanged);
    connect(m_gsd.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onGsdValueChanged);
    connect(m_width.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onWidthValueChanged);
    connect(m_runs.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onRunsChanged);

    connect(m_maxRoll.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onMaxRollValueChanged);
    connect(m_maneuverR.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onManeuverRValueChanged);
    connect(m_flightSpeed.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onFlightSpeedValueChanged);

    connect(m_uavModel.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onUavModelValueChanged);
    connect(m_cameraModel.get(), &Fact::valueChanged, this, &PhotoplannerEdit::onCameraModelValueChanged);

    for(int i = 0; i < m_uavParams->size(); i++)
        connect(m_uavParams->child(i), &Fact::valueChanged, this, &PhotoplannerEdit::onUavAnyParamChanged);

    for(int i = 0; i < m_cameraParams->size(); i++)
        connect(m_cameraParams->child(i), &Fact::valueChanged, this, &PhotoplannerEdit::onCameraAnyParamChanged);

    connect(m_applyButton.get(), &FactAction::triggered, this, &PhotoplannerEdit::applyClicked);

    writeDefaultUavAndCameraData();
    m_uavModel->setEnumStrings(getUavNames());
    m_cameraModel->setEnumStrings(getCameraNames());

    onUavModelValueChanged();
    onCameraModelValueChanged();
    onMissionTypeValueChanged();
    calcWidthAndRuns();
    calcAltitudeAndGsd();
    calcUavRAndRoll();
}

void PhotoplannerEdit::calcWidthAndRuns()
{
    int width = calcWidth();
    m_width->setValue(width);
}

int PhotoplannerEdit::calcWidth()
{
    auto model = getCameraModel();
    int width = model.CalcLinearWidth(m_altitude->value().toInt(),
                                      m_longitudinalOverlap->value().toInt(),
                                      m_transverseOverlap->value().toInt(),
                                      m_runs->value().toInt());
    return width;
}

int PhotoplannerEdit::calcRuns()
{
    auto model = getCameraModel();
    int runs = model.CalcLinearRuns(m_altitude->value().toInt(),
                                    m_longitudinalOverlap->value().toInt(),
                                    m_transverseOverlap->value().toInt(),
                                    m_width->value().toInt());
    return runs;
}

int PhotoplannerEdit::calcGsd()
{
    auto model = getCameraModel();
    int gsd = model.CalcGsd(m_altitude->value().toInt()) * 100;
    return gsd;
}

int PhotoplannerEdit::calcAltitude()
{
    auto model = getCameraModel();
    int altitude = model.CalcH(m_gsd->value().toInt());
    return altitude;
}

void PhotoplannerEdit::calcAltitudeAndGsd()
{
    int gsd = calcGsd();
    m_gsd->setValue(gsd);
}

void PhotoplannerEdit::calcUavRAndRoll()
{
    int maxRoll = calcUavRoll();
    m_maxRoll->setValue(maxRoll);
}

int PhotoplannerEdit::calcUavRoll()
{
    auto model = getUavModel();
    int maxRoll = aero_photo::R2D(model.CalcUavMaxRollOnManeuverR(m_maneuverR->value().toInt()));
    return maxRoll;
}

int PhotoplannerEdit::calcUavR()
{
    auto model = getUavModel();
    int uavR = model.CalcUavManeuverROnMaxRoll(aero_photo::D2R(m_maxRoll->value().toInt()));
    return uavR;
}

aero_photo::PhotoUavModel PhotoplannerEdit::getUavModel()
{
    aero_photo::PhotoUavModel model(m_flightSpeed->value().toInt(),
                                    aero_photo::D2R(m_maxRoll->value().toInt()));
    return model;
}

void PhotoplannerEdit::onMissionTypeValueChanged()
{
    if(m_missionType->value().toInt() == mtArea)
    {
        m_azimuth->setVisible(true);
        m_width->setVisible(false);
        m_runs->setVisible(false);
        m_reverseDirection->setVisible(false);
    }
    else if(m_missionType->value().toInt() == mtLinear)
    {
        m_azimuth->setVisible(false);
        m_width->setVisible(true);
        m_runs->setVisible(true);
        m_reverseDirection->setVisible(true);
    }
    else
        apxDebug() << "unknown mission type " << m_missionType->value();
}

void PhotoplannerEdit::onLongitudinalOverlapValueChanged()
{
    calcWidthAndRuns();
}

void PhotoplannerEdit::onTransverseOverlapValueChanged()
{
    calcWidthAndRuns();
}

void PhotoplannerEdit::onAltitudeValueChanged()
{
    setValueLikeANinja(m_gsd.get(), calcGsd());
}

void PhotoplannerEdit::onGsdValueChanged()
{
    setValueLikeANinja(m_altitude.get(), calcAltitude());
}

void PhotoplannerEdit::onWidthValueChanged()
{
    setValueLikeANinja(m_runs.get(), calcRuns());
}

void PhotoplannerEdit::onRunsChanged()
{
    setValueLikeANinja(m_width.get(), calcWidth());
}

void PhotoplannerEdit::onMaxRollValueChanged()
{
    setValueLikeANinja(m_maneuverR.get(), calcUavR());
}

void PhotoplannerEdit::onManeuverRValueChanged()
{
    setValueLikeANinja(m_maxRoll.get(), calcUavRoll());
}

void PhotoplannerEdit::onFlightSpeedValueChanged()
{
    calcUavRAndRoll();
}

void PhotoplannerEdit::onUavModelValueChanged()
{
    int idx = m_uavModel->value().toInt();
    loadUavData(idx);
}

void PhotoplannerEdit::onCameraModelValueChanged()
{
    int idx = m_cameraModel->value().toInt();
    loadCameraData(idx);
}

void PhotoplannerEdit::onUavAnyParamChanged()
{
    saveUavData();
    int idx = m_uavModel->value().toInt();
    if(m_uavName->value().toString() != m_uavModel->enumText(idx))
    {
        m_uavModel->setEnumStrings(getUavNames());
        m_uavModel->setValue(idx);
    }
}

void PhotoplannerEdit::onCameraAnyParamChanged()
{
    saveCameraData();
    int idx = m_cameraModel->value().toInt();
    if(m_cameraName->value().toString() != m_cameraModel->enumText(idx))
    {
        m_cameraModel->setEnumStrings(getCameraNames());
        m_cameraModel->setValue(idx);
    }
}

aero_photo::PhotoCameraModel PhotoplannerEdit::getCameraModel()
{
    aero_photo::PhotoCameraModel model(m_focusRange->value().toInt() / 100.0,
                                       m_sensorLx->value().toInt() / 100.0,
                                       m_sensorLy->value().toInt() / 100.0,
                                       m_sensorAx->value().toInt() / 100.0,
                                       m_sensorAy->value().toInt() / 100.0);
    return model;
}

int PhotoplannerEdit::getMissionType()
{
    return m_missionType->value().toInt();
}

int PhotoplannerEdit::getAltitude() const
{
    return m_altitude->value().toInt();
}

int PhotoplannerEdit::getAzimuth() const
{
    return m_azimuth->value().toInt();
}

int PhotoplannerEdit::getLongitudinalOverlap() const
{
    return m_longitudinalOverlap->value().toInt();
}

int PhotoplannerEdit::getTransverseOverlap() const
{
    return m_transverseOverlap->value().toInt();
}

bool PhotoplannerEdit::getExtentAlignment() const
{
    return m_extendAlignment->value().toBool();
}

bool PhotoplannerEdit::getManeuverAlignment() const
{
    return m_maneuverAlignment->value().toBool();
}

int PhotoplannerEdit::getWidth() const
{
    return m_width->value().toInt();
}

int PhotoplannerEdit::getVelocity() const
{
    return m_flightSpeed->value().toInt();
}

bool PhotoplannerEdit::getWithPhotoprints() const
{
    return m_withPhotoprints->value().toBool();
}

void PhotoplannerEdit::saveCameraData()
{
    int idx = m_cameraModel->value().toInt();

    QSettings *settings = AppSettings::settings();
    settings->beginGroup("photoplanner");
    QString str = QJsonDocument(m_cameraParams->toJson()).toJson(QJsonDocument::Compact);
    settings->setValue(QString("camera_%1").arg(idx), str);
    settings->endGroup();
}

void PhotoplannerEdit::loadCameraData(int idx)
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("photoplanner");
    QString str = settings->value(QString("camera_%1").arg(idx)).toString();
    settings->endGroup();

    m_cameraParams->fromJson(QJsonDocument::fromJson(str.toUtf8()).object());
}

void PhotoplannerEdit::saveUavData()
{
    int idx = m_uavModel->value().toInt();

    QSettings *settings = AppSettings::settings();
    settings->beginGroup("photoplanner");
    QString str = QJsonDocument(m_uavParams->toJson()).toJson(QJsonDocument::Compact);
    settings->setValue(QString("uav_%1").arg(idx), str);
    settings->endGroup();
}

void PhotoplannerEdit::loadUavData(int idx)
{
    QSettings *settings = AppSettings::settings();
    settings->beginGroup("photoplanner");
    QString str = settings->value(QString("uav_%1").arg(idx)).toString();
    settings->endGroup();

    m_uavParams->fromJson(QJsonDocument::fromJson(str.toUtf8()).object());
}

QStringList PhotoplannerEdit::getUavNames()
{
    auto settings = AppSettings::settings();
    settings->beginGroup("photoplanner");
    QStringList names;
    for(int i = 0; i < 6; i++)
    {
        QString key = QString("uav_%1").arg(i);
        QString str = settings->value(key).toString();
        auto object = QJsonDocument::fromJson(str.toUtf8()).object();
        names.append(object["uav_name"].toString());
    }
    settings->endGroup();
    return names;
}

QStringList PhotoplannerEdit::getCameraNames()
{
    auto settings = AppSettings::settings();
    QStringList names;
    settings->beginGroup("photoplanner");
    for(int i = 0; i < 7; i++)
    {
        QString key = QString("camera_%1").arg(i);
        QString str = settings->value(key).toString();
        auto object = QJsonDocument::fromJson(str.toUtf8()).object();
        names.append(object["camera_name"].toString());
    }
    settings->endGroup();
    return names;
}

void PhotoplannerEdit::writeDefaultUavAndCameraData()
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

    QList<QJsonObject> uavData = {
        {{"uav_name", "Plane 1"}, {"comm_radius", 25}, {"flight_speed", 15}, {"flight_time", 60}, {"max_roll", 30}},
        {{"uav_name", "Plane 2"}, {"comm_radius", 50}, {"flight_speed", 20}, {"flight_time", 120}, {"max_roll", 30}},
        {{"uav_name", "Plane 3"}, {"comm_radius", 60}, {"flight_speed", 20}, {"flight_time", 120}, {"max_roll", 40}},
        {{"uav_name", "Quadro 1"}, {"comm_radius", 5}, {"flight_speed", 10}, {"flight_time", 40}, {"max_roll", 90}},
        {{"uav_name", "Quadro 2"}, {"comm_radius", 15}, {"flight_speed", 30}, {"flight_time", 60}, {"max_roll", 90}},
        {{"uav_name", "Quadro 3"}, {"comm_radius", 15}, {"flight_speed", 30}, {"flight_time", 60}, {"max_roll", 80}},
    };

    QSettings *settings = AppSettings::settings();
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

    for(int i = 0; i < uavData.size(); i++)
    {
        QString key = QString("uav_%1").arg(i);
        QString str = settings->value(key).toString();
        if(str.isEmpty())
        {
            str = QJsonDocument(uavData[i]).toJson(QJsonDocument::Compact);
            settings->setValue(key, str);
        }
    }
    settings->endGroup();
}

void PhotoplannerEdit::setValueLikeANinja(FactData *data, const QVariant &value)
{
    if(m_ninja)
        return;
    m_ninja = true;
    data->setValue(value);
    m_ninja = false;
}

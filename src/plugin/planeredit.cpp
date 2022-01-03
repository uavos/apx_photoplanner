#include "planeredit.h"

#include "App/AppPrefs.h"
#include "App/AppLog.h"

PlanerEdit::PlanerEdit(CameraEdit *camera, Fact *parent):
    Fact(parent, "planner", "Planner", "", Section, "map"),
    m_cameraEdit(camera)
{
    f_missionType = new Fact(this, "mission_type", "Mission type", "", Enum, "map");
    f_uavModel = new Fact(this, "uav_model", "Uav model", "", Enum, "airplane");
    f_cameraModel = new Fact(this, "camera_model", "Camera model", "", Enum, "camera");
    f_extendAlignment = new Fact(this, "extend_alignment", "Extend alignment", "", Bool, "shape-polygon-plus");
    f_maneuverAlignment = new Fact(this, "maneuver_alignment", "Use maneuver for alignment", "", Bool, "call-missed");
    f_longitudinalOverlap = new Fact(this, "longitudinal_overlap", "Longitudinal overlap", "", Int, "vector-intersection");
    f_transverseOverlap = new Fact(this, "transverse_overlap", "Transverse overlap", "", Int, "vector-intersection");
    f_altitude = new Fact(this, "altitude", "Altitude", "", Int, "airplane-takeoff");
    f_gsd = new Fact(this, "gsd", "GSD", "", Float, "details");
    f_azimuth = new Fact(this, "azimuth", "Azimuth", "", Int, "compass");
    f_width = new Fact(this, "width", "Width", "", Int);
    f_runs = new Fact(this, "runs", "Runs", "", Int);
    f_reverseDirection = new Fact(this, "reverse_direction", "Reverse direction", "", Bool);
    f_withPhotoprints = new Fact(this, "with_photoprints", "With photoprints", "", Bool, "image-multiple");
    f_update = new Fact(this, "update", "Update", "", Fact::Action | Fact::Apply | Fact::CloseOnTrigger);

    f_missionType->setEnumStrings({"area", "linear"}, {mtArea, mtLinear});

    f_extendAlignment->setValue(true);
    f_maneuverAlignment->setValue(true);

    f_longitudinalOverlap->setMin(0);
    f_longitudinalOverlap->setMax(95);
    f_longitudinalOverlap->setUnits("%");

    f_transverseOverlap->setMin(0);
    f_transverseOverlap->setMax(95);
    f_transverseOverlap->setUnits("%");

    f_altitude->setMin(5);
    f_altitude->setMax(5000);
    f_altitude->setUnits("m");
    f_altitude->setValue(300);

    f_gsd->setMin(1);
    f_gsd->setMax(300);
    f_gsd->setUnits("sm/pix");

    f_azimuth->setMin(0);
    f_azimuth->setMax(359);
    f_azimuth->setUnits("deg");

    f_width->setMin(1);
    f_width->setMax(1000);
    f_width->setUnits("m");

    f_runs->setMin(1);
    f_runs->setMax(10);

    f_withPhotoprints->setValue(false);

    connect(f_missionType, &Fact::valueChanged, this, &PlanerEdit::onMissionTypeValueChanged);
    connect(f_longitudinalOverlap, &Fact::valueChanged, this, &PlanerEdit::onLongitudinalOverlapValueChanged);
    connect(f_transverseOverlap, &Fact::valueChanged, this, &PlanerEdit::onTransverseOverlapValueChanged);
    connect(f_altitude, &Fact::valueChanged, this, &PlanerEdit::onAltitudeValueChanged);
    connect(f_gsd, &Fact::valueChanged, this, &PlanerEdit::onGsdValueChanged);
    connect(f_width, &Fact::valueChanged, this, &PlanerEdit::onWidthValueChanged);
    connect(f_runs, &Fact::valueChanged, this, &PlanerEdit::onRunsChanged);

    connect(f_uavModel, &Fact::valueChanged, this, &PlanerEdit::onUavModelValueChanged);
    connect(f_cameraModel, &Fact::valueChanged, this, &PlanerEdit::onCameraModelValueChanged);

    f_uavModel->setEnumStrings(getUavNames());
    f_cameraModel->setEnumStrings(getCameraNames());

    onUavModelValueChanged();
    onCameraModelValueChanged();
    onMissionTypeValueChanged();
    calcWidthAndRuns();
    calcAltitudeAndGsd();
}

int PlanerEdit::getMissionType()
{
    return f_missionType->value().toInt();
}

int PlanerEdit::getAltitude() const
{
    return f_altitude->value().toInt();
}

int PlanerEdit::getAzimuth() const
{
    return f_azimuth->value().toInt();
}

int PlanerEdit::getLongitudinalOverlap() const
{
    return f_longitudinalOverlap->value().toInt();
}

int PlanerEdit::getTransverseOverlap() const
{
    return f_transverseOverlap->value().toInt();
}

bool PlanerEdit::getExtentAlignment() const
{
    return f_extendAlignment->value().toBool();
}

bool PlanerEdit::getManeuverAlignment() const
{
    return f_maneuverAlignment->value().toBool();
}

int PlanerEdit::getWidth() const
{
    return f_width->value().toInt();
}

bool PlanerEdit::getWithPhotoprints() const
{
    return f_withPhotoprints->value().toBool();
}

int PlanerEdit::calcWidth()
{
    auto model = m_cameraEdit->getCameraModel();
    int width = model.CalcLinearWidth(f_altitude->value().toInt(),
                                      f_longitudinalOverlap->value().toInt(),
                                      f_transverseOverlap->value().toInt(),
                                      f_runs->value().toInt());
    return width;
}

void PlanerEdit::calcWidthAndRuns()
{
    int width = calcWidth();
    f_width->setValue(width);
}

int PlanerEdit::calcRuns()
{
    auto model = m_cameraEdit->getCameraModel();
    int runs = model.CalcLinearRuns(f_altitude->value().toInt(),
                                    f_longitudinalOverlap->value().toInt(),
                                    f_transverseOverlap->value().toInt(),
                                    f_width->value().toInt());
    return runs;
}

int PlanerEdit::calcGsd()
{
    auto model = m_cameraEdit->getCameraModel();
    int gsd = model.CalcGsd(f_altitude->value().toInt()) * 100;
    return gsd;
}

int PlanerEdit::calcAltitude()
{
    auto model = m_cameraEdit->getCameraModel();
    int altitude = model.CalcH(f_gsd->value().toFloat() / 100.0);
    return altitude;
}

void PlanerEdit::calcAltitudeAndGsd()
{
    int gsd = calcGsd();
    f_gsd->setValue(gsd);
}

void PlanerEdit::setValueLikeANinja(FactData *data, const QVariant &value)
{
    if(m_ninja)
        return;
    m_ninja = true;
    data->setValue(value);
    m_ninja = false;
}

QStringList PlanerEdit::getUavNames()
{
    auto settings = AppPrefs::settings();
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

QStringList PlanerEdit::getCameraNames()
{
    auto settings = AppPrefs::settings();
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

void PlanerEdit::onMissionTypeValueChanged()
{
    if(f_missionType->value().toInt() == mtArea)
    {
        f_azimuth->setVisible(true);
        f_width->setVisible(false);
        f_runs->setVisible(false);
        f_reverseDirection->setVisible(false);
    }
    else if(f_missionType->value().toInt() == mtLinear)
    {
        f_azimuth->setVisible(false);
        f_width->setVisible(true);
        f_runs->setVisible(true);
        f_reverseDirection->setVisible(true);
    }
    else
        apxDebug() << "unknown mission type " << f_missionType->value();
}

void PlanerEdit::onLongitudinalOverlapValueChanged()
{
    calcWidthAndRuns();
}

void PlanerEdit::onTransverseOverlapValueChanged()
{
    calcWidthAndRuns();
}

void PlanerEdit::onAltitudeValueChanged()
{
    setValueLikeANinja(f_gsd, calcGsd());
}

void PlanerEdit::onGsdValueChanged()
{
    setValueLikeANinja(f_altitude, calcAltitude());
}

void PlanerEdit::onWidthValueChanged()
{
    setValueLikeANinja(f_runs, calcRuns());
}

void PlanerEdit::onRunsChanged()
{
    setValueLikeANinja(f_width, calcWidth());
}

void PlanerEdit::onUavModelValueChanged()
{
    int idx = f_uavModel->value().toInt();
    emit uavChanged(idx);
}

void PlanerEdit::onCameraModelValueChanged()
{
    int idx = f_cameraModel->value().toInt();
    emit cameraChanged(idx);
}

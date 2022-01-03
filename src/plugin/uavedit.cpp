#include "uavedit.h"

#include "PhotoGeoCalcs.h"
#include "App/AppPrefs.h"

UavEdit::UavEdit(Fact *parent):
    Fact(parent, "uav", "Uav", "", Section, "airplane")
{
    f_uavName = new Fact(this, "uav_name", "Name", "", Text, "airplane");
    f_flightTime = new Fact(this, "flight_time", "Flight time", "", Int, "clock");
    f_flightSpeed = new Fact(this, "flight_speed", "Flight speed", "", Int, "speedometer");
    f_useSpeedInWaypoint = new Fact(this, "use_speed_int_waypoint", "Use speed in waypoint", "", Bool, "check");
    f_commRadius = new Fact(this, "comm_radius", "Comm radius", "", Int, "vector-radius");
    f_maxRoll = new Fact(this, "max_roll", "Max roll", "", Int, "rotate-right");
    f_maneuverR = new Fact(this, "maneuver_r", "Maneuver R", "", Int, "map-marker-distance");
    f_update = new Fact(this, "update", "Update", "", Fact::Action | Fact::Apply | Fact::CloseOnTrigger);

    f_flightTime->setMin(1);
    f_flightTime->setMax(240);
    f_flightTime->setUnits("min");

    f_flightSpeed->setMin(1);
    f_flightSpeed->setMax(200);
    f_flightSpeed->setUnits("m/s");

    f_useSpeedInWaypoint->setValue(false);

    f_commRadius->setMin(1);
    f_commRadius->setMax(120);
    f_commRadius->setUnits("km");

    f_maxRoll->setMin(1);
    f_maxRoll->setMax(90);
    f_maxRoll->setUnits("deg");

    f_maneuverR->setMin(0);
    f_maneuverR->setMax(5000);
    f_maneuverR->setUnits("m");


    connect(f_maxRoll, &Fact::valueChanged, this, &UavEdit::onMaxRollValueChanged);
    connect(f_maneuverR, &Fact::valueChanged, this, &UavEdit::onManeuverRValueChanged);
    connect(f_flightSpeed, &Fact::valueChanged, this, &UavEdit::onFlightSpeedValueChanged);

    calcUavRAndRoll();
    writeDefaultUavData();

    loadData();

    for(int i = 0; i < size(); i++)
        connect(child(i), &Fact::valueChanged, this, &UavEdit::saveUavData);
}

int UavEdit::getVelocity() const
{
    return f_flightSpeed->value().toInt();
}

bool UavEdit::getUseSpeedInWaypoint() const
{
    return f_useSpeedInWaypoint->value().toBool();
}

aero_photo::PhotoUavModel UavEdit::getUavModel()
{
    aero_photo::PhotoUavModel model(f_flightSpeed->value().toInt(),
                                    qDegreesToRadians(qreal(f_maxRoll->value().toInt())));
    return model;
}

void UavEdit::onCurrentUavChanged(int index)
{
    m_index = index;
    loadData();
}

void UavEdit::calcUavRAndRoll()
{
    int maxRoll = calcUavRoll();
    f_maxRoll->setValue(maxRoll);
}

int UavEdit::calcUavRoll()
{
    auto model = getUavModel();
    int maxRoll = qRadiansToDegrees(model.CalcUavMaxRollOnManeuverR(f_maneuverR->value().toInt()));
    return maxRoll;
}

int UavEdit::calcUavR()
{
    auto model = getUavModel();
    int uavR = model.CalcUavManeuverROnMaxRoll(qDegreesToRadians(qreal(f_maxRoll->value().toInt())));
    return uavR;
}

void UavEdit::loadData()
{
    auto settings = AppPrefs::instance()->settings();
    settings->beginGroup("photoplanner");
    auto str = settings->value(QString("uav_%1").arg(m_index)).toString();
    auto j = QJsonDocument::fromJson(str.toUtf8()).object();
    valuesFromJson(j);
    settings->endGroup();
}

void UavEdit::setValueLikeANinja(FactData *data, const QVariant &value)
{
    if(m_ninja)
        return;
    m_ninja = true;
    data->setValue(value);
    m_ninja = false;
}

void UavEdit::saveUavData()
{
    QSettings *settings = AppPrefs::settings();
    settings->beginGroup("photoplanner");
    QString str = QJsonDocument(valuesToJson()).toJson(QJsonDocument::Compact);
    settings->setValue(QString("uav_%1").arg(m_index), str);
    settings->endGroup();
}

void UavEdit::writeDefaultUavData()
{
    QList<QJsonObject> uavData = {
        {{"uav_name", "Plane 1"}, {"comm_radius", 25}, {"flight_speed", 15}, {"flight_time", 60}, {"max_roll", 30}},
        {{"uav_name", "Plane 2"}, {"comm_radius", 50}, {"flight_speed", 20}, {"flight_time", 120}, {"max_roll", 30}},
        {{"uav_name", "Plane 3"}, {"comm_radius", 60}, {"flight_speed", 20}, {"flight_time", 120}, {"max_roll", 40}},
        {{"uav_name", "Quadro 1"}, {"comm_radius", 5}, {"flight_speed", 10}, {"flight_time", 40}, {"max_roll", 90}},
        {{"uav_name", "Quadro 2"}, {"comm_radius", 15}, {"flight_speed", 30}, {"flight_time", 60}, {"max_roll", 90}},
        {{"uav_name", "Quadro 3"}, {"comm_radius", 15}, {"flight_speed", 30}, {"flight_time", 60}, {"max_roll", 80}},
    };

    QSettings *settings = AppPrefs::settings();
    settings->beginGroup("photoplanner");

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
    settings->sync();
}

void UavEdit::onMaxRollValueChanged()
{
    setValueLikeANinja(f_maneuverR, calcUavR());
}

void UavEdit::onManeuverRValueChanged()
{
    setValueLikeANinja(f_maxRoll, calcUavRoll());
}

void UavEdit::onFlightSpeedValueChanged()
{
    calcUavRAndRoll();
}

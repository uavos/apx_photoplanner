#include "pointedit.h"

PointEdit::PointEdit(Fact *parent, int id, QGeoCoordinate coordinate):
    Fact(parent, "point_edit", "Point edit", "", Group),
    m_id(id),
    m_coordinate(coordinate),
    m_latitude(new Fact(this, "latitude", "Latitude", "", Float)),
    m_longitude(new Fact(this, "longitude", "Longitude", "", Float)),
    m_update(new FactAction(this, "update", "Update", "", "", FactAction::ActionApply)),
    m_remove(new FactAction(this, "remove", "Remove", "", "", FactAction::ActionRemove))
{
    m_latitude->setUnits("lat");
    m_longitude->setUnits("lon");

    m_latitude->setValue(coordinate.latitude());
    m_longitude->setValue(coordinate.longitude());

    connect(m_update.get(), &FactAction::triggered, this, &PointEdit::onUpdateActionTriggered);
    connect(m_remove.get(), &FactAction::triggered, this, &PointEdit::onRemoveActionTriggered);
}

void PointEdit::onUpdateActionTriggered()
{
    QGeoCoordinate coordinate(m_latitude->value().toDouble(), m_longitude->value().toDouble());
    emit updatePointTriggered(m_id, coordinate);
}

void PointEdit::onRemoveActionTriggered()
{
    emit removePointTriggered(m_id);
}

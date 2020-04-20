#ifndef POINTEDIT_H
#define POINTEDIT_H

#include <memory>
#include <QGeoCoordinate>
#include "Fact/Fact.h"

class PointEdit: public Fact
{
    Q_OBJECT
public:
    PointEdit(Fact *parent, int id, QGeoCoordinate coordinate);

private:
    int m_id;
    QGeoCoordinate m_coordinate;
    std::unique_ptr<Fact> m_latitude;
    std::unique_ptr<Fact> m_longitude;
    std::unique_ptr<Fact> m_update;
    std::unique_ptr<Fact> m_remove;

private slots:
    void onUpdateActionTriggered();
    void onRemoveActionTriggered();

signals:
    void updatePointTriggered(int id, QGeoCoordinate coordinate);
    void removePointTriggered(int id);
};

#endif // POINTEDIT_H

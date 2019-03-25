#ifndef PHOTOPLANNER_H
#define PHOTOPLANNER_H

#include <QObject>
#include <memory>
#include "Fact.h"
#include "points.h"

class Photoplanner: public Fact
{
    Q_OBJECT
    Q_PROPERTY(Points *points READ getPoints CONSTANT)
public:
    Photoplanner(Fact *parent = nullptr);
    Points* getPoints();

public slots:
    void onLoadingFinished();
    void onAddPhotoplannerPointTriggered();

private:
    std::unique_ptr<Points> m_points;
    std::unique_ptr<Fact> m_addPhotoplannerPoint;
};

#endif // PHOTOPLANNER_H

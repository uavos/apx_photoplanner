#ifndef PHOTOPLANNER_H
#define PHOTOPLANNER_H

#include <QObject>
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

private:
    Points *m_points;
};

#endif // PHOTOPLANNER_H

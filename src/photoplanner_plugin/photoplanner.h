#ifndef APXPHOTOPLANNER_H
#define APXPHOTOPLANNER_H

#include <QObject>
#include <memory>
#include "Fact/Fact.h"
#include "borderpoints.h"
#include "photoprints.h"
#include "PhotoUavModel.h"
#include "PhotoCameraModel.h"
#include "photoplanneredit.h"
#include "pointedit.h"

class ApxPhotoplanner: public Fact
{
    Q_OBJECT
    Q_PROPERTY(BorderPoints *borderPoints READ getBorderPoints CONSTANT)
    Q_PROPERTY(PhotoPrints *photoPrints READ getPhotoPrints CONSTANT)
    Q_PROPERTY(QString missionType READ getMissionType NOTIFY missionTypeChanged)
    Q_PROPERTY(uint totalDistance READ getTotalDistance NOTIFY totalDistanceChanged)
public:
    ApxPhotoplanner(Fact *parent = nullptr);
    BorderPoints* getBorderPoints();
    PhotoPrints* getPhotoPrints();
    QString getMissionType() const;
    uint getTotalDistance() const;
    Q_INVOKABLE void createEditor(int id, QGeoCoordinate coordinate);

private:
    uint m_totalDistance;
    std::unique_ptr<BorderPoints> m_borderPoints;
    std::unique_ptr<PhotoPrints> m_photoPrints;
    std::unique_ptr<Fact> m_addPhotoplannerPoint;
    std::unique_ptr<PhotoplannerEdit> m_photoplannerEdit;
    std::unique_ptr<PointEdit> m_pointEdit;
    aero_photo::PhotoCameraModel m_cameraModel;
    aero_photo::PhotoUavModel m_uavModel;

private slots:
    void onLoadingFinished();
    void onAddPhotoplannerPointTriggered();
    void onBorderPointsRowsInserted(const QModelIndex &parent, int first, int last);
    void onBorderPointsRowsRemoved(const QModelIndex &parent, int first, int last);
    void onBorderPointsDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void calculatePhotoPlan();

signals:
    void missionTypeChanged();
    void totalDistanceChanged();
};

#endif // APXPHOTOPLANNER_H

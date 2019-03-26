#ifndef APXPHOTOPLANNER_H
#define APXPHOTOPLANNER_H

#include <QObject>
#include <memory>
#include "Fact.h"
#include "borderpoints.h"
#include "photoprints.h"
#include "PhotoUavModel.h"
#include "PhotoCameraModel.h"

class ApxPhotoplanner: public Fact
{
    Q_OBJECT
    Q_PROPERTY(BorderPoints *borderPoints READ getBorderPoints CONSTANT)
    Q_PROPERTY(PhotoPrints *photoPrints READ getPhotoPrints CONSTANT)
public:
    ApxPhotoplanner(Fact *parent = nullptr);
    BorderPoints* getBorderPoints();
    PhotoPrints* getPhotoPrints();

public slots:
    void onLoadingFinished();
    void onAddPhotoplannerPointTriggered();

private:
    std::unique_ptr<BorderPoints> m_borderPoints;
    std::unique_ptr<PhotoPrints> m_photoPrints;
    std::unique_ptr<Fact> m_addPhotoplannerPoint;
    aero_photo::PhotoCameraModel m_cameraModel;
    aero_photo::PhotoUavModel m_uavModel;

    void calculatePhotoPlan();

    void onBorderPointsRowsInserted(const QModelIndex &parent, int first, int last);
    void onBorderPointsDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
};

#endif // APXPHOTOPLANNER_H

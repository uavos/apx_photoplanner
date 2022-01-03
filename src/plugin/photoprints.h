#pragma once

#include <QAbstractListModel>
#include <QGeoCoordinate>

class PhotoPrints: public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        P1Role = Qt::UserRole + 1,
        P2Role,
        P3Role,
        P4Role
    };
    PhotoPrints();
    void setPrints(const QVector<QVector<QGeoCoordinate>> &prints);

protected:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

private:
    QVector<QVector<QGeoCoordinate>> m_prints;
    QModelIndex m_rootIndex;
};

#ifndef BORDERPOINTS_H
#define BORDERPOINTS_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

class BorderPoints: public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        Id = Qt::UserRole + 1,
        CoordinateRole
    };
    BorderPoints();
    int appendPoint(const QGeoCoordinate &point);
    void updatePoint(int id, const QGeoCoordinate &point);
    void removePoint(int id);
    QVector<QGeoCoordinate> getAllPoints() const;
    Q_INVOKABLE QVariantList getAllPointsAsVariants() const;

protected:
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

private:
    QVector<QGeoCoordinate> m_points;
    QModelIndex m_rootIndex;

signals:
    void pointsChanged();
};

#endif // BORDERPOINTS_H

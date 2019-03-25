#ifndef POINTS_H
#define POINTS_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

class Points: public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        LatRole = Qt::UserRole + 1,
        LonRole
    };
    Points();
    void appendPoint(const QGeoCoordinate &point);
    QVector<QGeoCoordinate> getAllPoints() const;

protected:
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

private:
    QVector<QGeoCoordinate> m_points;
    QModelIndex m_rootIndex;
};

#endif // POINTS_H

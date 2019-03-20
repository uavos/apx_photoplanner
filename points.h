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

protected:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

private:
    QVector<QGeoCoordinate> m_points;
};

#endif // POINTS_H

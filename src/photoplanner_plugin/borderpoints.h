#ifndef BORDERPOINTS_H
#define BORDERPOINTS_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

class BorderPoints: public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        CoordinateRole = Qt::UserRole + 1
    };
    BorderPoints();
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

#endif // BORDERPOINTS_H

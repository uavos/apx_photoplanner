#include "points.h"

Points::Points()
{
}

void Points::appendPoint(const QGeoCoordinate &point)
{
    beginResetModel();
    m_points.append(point);
    endResetModel();
}

int Points::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent); //flat list
    return m_points.size();
}

QVariant Points::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < m_points.size())
    {
        if(role == LatRole)
            return m_points[row].latitude();
        else if(role == LonRole)
            return m_points[row].longitude();
    }
    return QVariant();
}

QHash<int, QByteArray> Points::roleNames() const
{
    QHash<int, QByteArray> rolenames;
    rolenames[LatRole] = "lat";
    rolenames[LonRole] = "lon";
    return rolenames;
}

#include "points.h"

Points::Points()
{
}

void Points::appendPoint(const QGeoCoordinate &point)
{
    beginInsertRows(m_rootIndex, m_points.size(), m_points.size());
    m_points.append(point);
    endInsertRows();
}

QVector<QGeoCoordinate> Points::getAllPoints() const
{
    return m_points;
}

Qt::ItemFlags Points::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEditable;
}

bool Points::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    if(row >= 0 && row < m_points.size())
    {
        if(role == LatRole)
        {
            m_points[row].setLatitude(value.toDouble());
            return true;
        }
        else if(role == LonRole)
        {
            m_points[row].setLongitude(value.toDouble());
            return true;
        }
    }
    return false;
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

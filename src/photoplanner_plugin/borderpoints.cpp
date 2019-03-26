#include "borderpoints.h"

BorderPoints::BorderPoints()
{
}

void BorderPoints::appendPoint(const QGeoCoordinate &point)
{
    beginInsertRows(m_rootIndex, m_points.size(), m_points.size());
    m_points.append(point);
    endInsertRows();
}

QVector<QGeoCoordinate> BorderPoints::getAllPoints() const
{
    return m_points;
}

Qt::ItemFlags BorderPoints::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEditable;
}

bool BorderPoints::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    if(row >= 0 && row < m_points.size())
    {
        if(role == CoordinateRole)
        {
            m_points[row] = value.value<QGeoCoordinate>();
            emit dataChanged(createIndex(row, 0), createIndex(row, 0), {CoordinateRole});
            return true;
        }
    }
    return false;
}

int BorderPoints::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent); //flat list
    return m_points.size();
}

QVariant BorderPoints::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < m_points.size())
    {
        if(role == CoordinateRole)
            return QVariant::fromValue(m_points[row]);
    }
    return QVariant();
}

QHash<int, QByteArray> BorderPoints::roleNames() const
{
    QHash<int, QByteArray> rolenames = {
        {CoordinateRole, "coord"}
    };
    return rolenames;
}

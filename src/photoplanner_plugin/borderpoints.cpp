#include "borderpoints.h"

#include <QDebug>

BorderPoints::BorderPoints()
{
    connect(this, &BorderPoints::rowsInserted, this, &BorderPoints::pointsChanged);
    connect(this, &BorderPoints::rowsRemoved, this, &BorderPoints::pointsChanged);
    connect(this, &BorderPoints::dataChanged, this, &BorderPoints::pointsChanged);
}

int BorderPoints::appendPoint(const QGeoCoordinate &point)
{
    beginInsertRows(m_rootIndex, m_points.size(), m_points.size());
    m_points.append(point);
    endInsertRows();
    return m_points.size() - 1;
}

void BorderPoints::updatePoint(int id, const QGeoCoordinate &point)
{
    m_points[id] = point;
    emit dataChanged(createIndex(id, 0), createIndex(id, 0), {CoordinateRole});
}

void BorderPoints::removePoint(int id)
{
    beginRemoveRows(m_rootIndex, id, id);
    m_points.removeAt(id);
    endRemoveRows();
}

QVector<QGeoCoordinate> BorderPoints::getAllPoints() const
{
    return m_points;
}

QVariantList BorderPoints::getAllPointsAsVariants() const
{
    QVariantList result;
    for(auto p: m_points)
        result.append(QVariant::fromValue(p));
    return result;
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
        if(role == Id)
            return QVariant::fromValue(row);
        else if(role == CoordinateRole)
            return QVariant::fromValue(m_points[row]);
    }
    return QVariant();
}

QHash<int, QByteArray> BorderPoints::roleNames() const
{
    QHash<int, QByteArray> rolenames = {
        {Id, "id"},
        {CoordinateRole, "coord"}
    };
    return rolenames;
}

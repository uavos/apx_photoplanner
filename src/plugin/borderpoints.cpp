#include "borderpoints.h"

#include <math.h>
#include <QtMath>

using namespace std::placeholders;

BorderPoints::BorderPoints()
{
    connect(this, &BorderPoints::rowsInserted, this, &BorderPoints::pointsChanged);
    connect(this, &BorderPoints::rowsRemoved, this, &BorderPoints::pointsChanged);
    connect(this, &BorderPoints::dataChanged, this, &BorderPoints::pointsChanged);
    connect(this, &BorderPoints::rowsInserted, this, &BorderPoints::areaChanged);
    connect(this, &BorderPoints::rowsRemoved, this, &BorderPoints::areaChanged);
    connect(this, &BorderPoints::dataChanged, this, &BorderPoints::areaChanged);
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

uint BorderPoints::getArea() const
{
    if(m_points.size() < 3)
        return 0;
    QVector<QGeoCoordinate> points;
    QVector<QPointF> reprojectedPoints;
    std::transform(m_points.begin(), m_points.end(), std::back_inserter(reprojectedPoints), &BorderPoints::reproject);
    double area = 0;
    for(int i = 0; i < reprojectedPoints.size(); i++)
    {
        int im1, ip1;
        if(i == 0)
        {
            im1 = reprojectedPoints.size() - 1;
            ip1 = i + 1;
        }
        else if(i == reprojectedPoints.size() - 1)
        {
            im1 = i - 1;
            ip1 = 0;
        }
        else
        {
            im1 = i - 1;
            ip1 = i + 1;
        }
        auto x = reprojectedPoints[i].x();
        auto yp1 = reprojectedPoints[ip1].y();
        auto ym1 = reprojectedPoints[im1].y();
        area = area + (x * (yp1 - ym1));
    }
    area = std::abs(area) / 2;
    return area;
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

QPointF BorderPoints::reproject(const QGeoCoordinate &c)
{
    const float EARTH_RADIUS = 6371009;
    const float LAT_DIST = M_PI * EARTH_RADIUS / 180.0;
    float y = c.latitude() * LAT_DIST;
    float x = c.longitude() * LAT_DIST * std::cos(qDegreesToRadians(c.latitude()));
    return QPointF(x, y);
}



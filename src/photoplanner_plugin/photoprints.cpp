#include "photoprints.h"

#include <iostream>

PhotoPrints::PhotoPrints()
{
}

void PhotoPrints::setPrints(const QVector<QVector<QGeoCoordinate> > &prints)
{
    beginResetModel();
    m_prints = prints;
    endResetModel();
}

Qt::ItemFlags PhotoPrints::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEditable;
}

//bool PhotoPrints::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    int row = index.row();
//    if(row >= 0 && row < m_prints.size())
//    {
//        if(role == LatRole)
//        {
//            m_prints[row].setLatitude(value.toDouble());
//            return true;
//        }
//        else if(role == LonRole)
//        {
//            m_prints[row].setLongitude(value.toDouble());
//            return true;
//        }
//    }
//    return false;
//}

int PhotoPrints::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent); //flat list
    return m_prints.size();
}

QVariant PhotoPrints::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < m_prints.size())
    {
        if(role == P1Role)
            return QVariant::fromValue(m_prints[row][0]);
        else if(role == P2Role)
            return QVariant::fromValue(m_prints[row][1]);
        else if(role == P3Role)
            return QVariant::fromValue(m_prints[row][3]); //0-1-3-2: not a bug
        else if(role == P4Role)
            return QVariant::fromValue(m_prints[row][2]);
    }
    return QVariant();
}

QHash<int, QByteArray> PhotoPrints::roleNames() const
{
    QHash<int, QByteArray> rolenames = {
        {P1Role, "p1"},
        {P2Role, "p2"},
        {P3Role, "p3"},
        {P4Role, "p4"}
    };
    return rolenames;
}

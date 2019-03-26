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

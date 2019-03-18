#include <QtMath>
#include <QFuture>


#include<QtConcurrent/QtConcurrentMap>

#include "spatialpointmodel.h"

static int global_pt_ID = 0;

SpatialPoint::SpatialPoint()
{
    m_point(0,0) = 0.0; m_point(0,1) = 0.0; m_point(0,2) = 0.0; m_ID = global_pt_ID++;
}

SpatialPoint::SpatialPoint(const qreal &X, const qreal &Y, const qreal &Z)
{
    m_point(0,0) = X; m_point(0,1) = Y; m_point(0,2) = Z; m_ID = global_pt_ID++;
}

SpatialPointModel::SpatialPointModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void SpatialPointModel::addPoint(const QVariant &X, const QVariant &Y, const QVariant &Z)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_SpatialPts.append( SpatialPoint(X.toReal(),Y.toReal(),Z.toReal()) );
    endInsertRows();
}

int SpatialPointModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_SpatialPts.count();
}

QVariant SpatialPointModel::data(const QModelIndex &index, int role) const
{
    // Check if the index provided is valid
    if( (index.row() < 0) || (index.row() >= m_SpatialPts.count()) )
        return QVariant();

    // Retrieve the entry at the specified index
    const SpatialPoint& pt = m_SpatialPts[index.row()];

    // Return the value requested as per the role specified
    switch(role){
    case XRole:
        return pt.X();
    case YRole:
        return pt.Y();
    case ZRole:
        return pt.Z();
    default:
        return QVariant();
    }
}


QHash<int, QByteArray> SpatialPointModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[XRole] = "X";
    roles[YRole] = "Y";
    roles[ZRole] = "Z";
    roles[AngleRole] = "Angle";
    return roles;
}


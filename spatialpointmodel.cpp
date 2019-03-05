#include "spatialpointmodel.h"

SpatialPointModel::SpatialPointModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void SpatialPointModel::addPoint(const QVariant &X, const QVariant &Y, const QVariant &Z)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_SpatialPts.append( SpatialPoint(X.toFloat(),Y.toFloat(),Z.toFloat()) );
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

    return roles;
}

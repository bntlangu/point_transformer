#include <QtMath>
#include <QFuture>
#include<QtConcurrent/QtConcurrentMap>

#include "spatialpointmodel.h"

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

void SpatialPointModel::clearModel()
{
    beginResetModel();
    m_SpatialPts.resize(0);
    endResetModel();
}

/*
 * This function performs the transformation of the residing set of points according to the
 * member transform matrices, which are presumed to have been set prior to this function being
 * called).
*/
void SpatialPointModel::transformModel(bool automate)
{


    if(automate){
        /*while(1){
            beginResetModel();
            QFuture<void> temp;
            switch (m_RotationAxis) {
            case X_AXIS:
                temp = QtConcurrent::map ( m_SpatialPts, this->transformModelX );
                break;
            case Y_AXIS:
                temp = QtConcurrent::map ( m_SpatialPts, this->transformModelY );
                break;
            case Z_AXIS:
                temp = QtConcurrent::map ( m_SpatialPts, this->transformModelZ );
                break;
            case ALL_AXES:
                temp = QtConcurrent::map ( m_SpatialPts, this->transformModel3D );
                break;
            }
            endResetModel();
        }*/
        // <TODO> Partition the data to allocate to the requisitie number of threads

    }else{
        beginResetModel();

        for ( int i = 0; i < m_SpatialPts.size(); i++ ){
            switch (m_RotationAxis) {
            case X_AXIS:
                transformModelX(m_SpatialPts[i]);
                break;
            case Y_AXIS:
                transformModelY(m_SpatialPts[i]);
                break;
            case Z_AXIS:
                transformModelZ(m_SpatialPts[i]);
                break;
            case ALL_AXES:
                transformModel3D(m_SpatialPts[i]);
                break;
            }
        }

        endResetModel();
    }
}

void SpatialPointModel::transformModelX(SpatialPoint &pt)
{
    pt.transformPoint(m_xAxisRotation);
}

void SpatialPointModel::transformModelY(SpatialPoint &pt)
{
    pt.transformPoint(m_yAxisRotation);
}

void SpatialPointModel::transformModelZ(SpatialPoint &pt)
{
    pt.transformPoint(m_zAxisRotation);
}

void SpatialPointModel::transformModel3D( SpatialPoint &pt )
{
    pt.transformPoint(m_xAxisRotation);
    pt.transformPoint(m_yAxisRotation);
    pt.transformPoint(m_zAxisRotation);
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

/*
 * This function is to be used for setting the transformation matrices that will be applied
 * to the residing point set when tranformModel is called. The transformation matrices for rotation
 * about each axis can either be individually set by calling this function with the corresponding axis
 * specified or the same angle can be applied to all the axes with one call specifiying the axis parameter
 * as "ALL_AXES".
*/
void SpatialPointModel::setRotationAngle(const qreal &angle, const ROTATION_AXIS &axis)
{
    m_RotationAxis = axis;

    if( (axis == X_AXIS) || (axis == ALL_AXES) ){
        m_xAngle = angle;
        m_xAxisRotation(0,0) = 1;
        m_xAxisRotation(0,1) = 0;
        m_xAxisRotation(0,2) = 0;
        m_xAxisRotation(1,0) = 0;
        m_xAxisRotation(1,1) = cos(angle);
        m_xAxisRotation(1,2) = sin(angle);
        m_xAxisRotation(2,0) = 0;
        m_xAxisRotation(2,1) = -sin(angle);
        m_xAxisRotation(2,2) = cos(angle);
    }

    if( (axis == Y_AXIS) || (axis == ALL_AXES) ){
        m_yAngle = angle;
        m_yAxisRotation(0,0) = cos(angle);
        m_yAxisRotation(0,1) = 0;
        m_yAxisRotation(0,2) = -sin(angle);
        m_yAxisRotation(1,0) = 0;
        m_yAxisRotation(1,1) = 1;
        m_yAxisRotation(1,2) = 0;
        m_yAxisRotation(2,0) = sin(angle);
        m_yAxisRotation(2,1) = 0;
        m_yAxisRotation(2,2) = cos(angle);
    }

    if( (axis == Z_AXIS) || (axis == ALL_AXES) ){
        m_zAngle = angle;
        m_zAxisRotation(0,0) = cos(angle);
        m_zAxisRotation(0,1) = sin(angle);
        m_zAxisRotation(0,2) = 0;
        m_zAxisRotation(1,0) = -sin(angle);
        m_zAxisRotation(1,1) = cos(angle);
        m_zAxisRotation(1,2) = 0;
        m_zAxisRotation(2,0) = 0;
        m_zAxisRotation(2,1) = 0;
        m_zAxisRotation(2,2) = 1;
    }
}

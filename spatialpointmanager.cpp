#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>
#include "spatialpointmanager.h"

SpatialPointManager::SpatialPointManager(QObject *parent) : QObject(parent), m_SimStatus(INACTIVE), m_Paused(false)
{
    // Populate with dummy data
    m_ptsModel.addPoint(-10.0, 5.0, -5.0);
    m_ptsModel.addPoint(-9.0, 3.0, -4.5);
    m_ptsModel.addPoint(-8.5, 4.1, -4.0);
    m_ptsModel.addPoint(-8.0, 4.75, -3.9);
    m_ptsModel.addPoint(-9.5, 4.9, -4.2);
    m_ptsModel.addPoint(-9.9, 3.42, -3.5);
    m_ptsModel.addPoint(-7.8, 3.1, -4.9);
    m_ptsModel.addPoint(-7.3, 2.91, -4.1);
    m_ptsModel.addPoint(-7.1, 3.68, -4.52);
    m_ptsModel.addPoint(-8.8, 2.96, -3.6);
    m_ptsModel.addPoint(-6.94, 2.4, -2.92);
    m_ptsModel.addPoint(-9.02, 4.74, -4.18);
    m_ptsModel.addPoint(-9.54, 3.1, -3.8);
    m_ptsModel.addPoint(-6.86, 3.66, -3.58);
    m_ptsModel.addPoint(-8.16, 1.82, -4.64);
    m_ptsModel.addPoint(-7.4, 3.18, -4.22);
    m_ptsModel.addPoint(-7.9, 3.06, -4.3);
    m_ptsModel.addPoint(-8.98, 2.64, -4.44);
    m_ptsModel.addPoint(-6.36, 3.96, -4.38);
    m_ptsModel.addPoint(-7.18, 3.32, -4.04);
    m_ptsModel.addPoint(-7.9, 3.4, -2.78);
    m_ptsModel.addPoint(-7.4, 3.12, -3.1);
    m_ptsModel.addPoint(-7.54, 2.8, -3.68);

    QObject::connect(&m_FutureWatcher, &QFutureWatcher<void>::finished, this, &SpatialPointManager::onFinished);
    QObject::connect(&m_FutureWatcher, &QFutureWatcher<void>::progressRangeChanged, this, &SpatialPointManager::onProgressRangeChanged);
    QObject::connect(&m_FutureWatcher, &QFutureWatcher<void>::progressValueChanged,  this, &SpatialPointManager::onProgressValueChanged);
}

/*
 * This function is used to create a simple line to be used as dummy data.
*/
void SpatialPointManager::createLine(const QVariant &num_pts, const QVariant &origin, const QVariant &shape_idx)
{
    clearModel();

    float x = origin.toInt(), y = origin.toInt(), z = origin.toInt();

    for (int i = 0; i < num_pts.toInt(); i++){
        m_ptsModel.addPoint(x, y, z);

        if(shape_idx == HORIZONTAL_LINE){
            x += 1;
        }else if (shape_idx == VERTICAL_LINE){
            y += 1;
        }
    }
}

/*
 * This function is used to create a random point cloud to be used as dummy data.
*/
void SpatialPointManager::createRandomPts(const QVariant &num_pts, const QVariant &max_val)
{
    clearModel();

    qreal x = 0, y = 0, z = 0;
    qreal fMax = max_val.toReal();

    for( int i = 0; i < num_pts.toInt(); i++ )
    {
        // We start by generating a double on the range [0, 1), we multiply it by 2
        // to get it on the range [0, 2), then subtract 1 resulting in a number
        // lying in the range [-1, 1). Then we multiply this result by fMax
        // to hopefully give a number lying on our esired range of [-Max,Max).
        x = (QRandomGenerator::global()->generateDouble()*2 - 1) * fMax;
        y = (QRandomGenerator::global()->generateDouble()*2 - 1) * fMax;
        z = (QRandomGenerator::global()->generateDouble()*2 - 1) * fMax;

        m_ptsModel.addPoint(x, y, z);
    }
}

/*
 * This function is used apply a color transformation to the set of points
 * Here we make the assumption that the input value coming in from
 * the UI is on the range [0,16777216] i.e. 24-bit color (2^24).
*/
void SpatialPointManager::applyColorTransform(const QVariant &colorVal)
{
    uint32_t uColorVal = colorVal.toUInt()*10;

    // Convert integer value into hex
    QString strColor = QString("#%1").arg(uColorVal, 6, 16, QLatin1Char( '0' ));

    // Push the hex string back to the UI
    emit updateColor(strColor);
}

/*
 * This function is used to apply a size transformation to the current set of
 * points. Here we make the assumption that the input value coming in from
 * the UI is on the range [0,1].
*/
void SpatialPointManager::applySizeTransform(const QVariant &sizeVal)
{
    float fSize = sizeVal.toFloat();

    // Push the hex string back to the UI
    emit updateSize(fSize);
}

// Here we make the assumption that the input value coming in from
// the UI is on the range [0,360] i.e. angle in degrees.
void SpatialPointManager::applyRotationTransform(const QVariant &angle, const QVariant &axis)
{
    // Convert integer value to angle (radians)
    setRotationAngle( qDegreesToRadians(angle.toReal()), ROTATION_AXIS(axis.toInt()));
    transformModel();

    qDebug() << "(applyRotationTransform) Angle: " << angle << "; Axis: " << axis;
}

void SpatialPointManager::receivePlay(const QVariant &angle, const QVariant &axis, const QVariant &num_cycles)
{
    qDebug() << "(receivePlay) RECEIVED PLAY SIGNAL!!!";

    if(m_SimStatus == PAUSED){
        m_FutureWatcher.resume();
        qDebug() << "(receivePlay) RESUME => Angle: " << angle << "; Axis: " << axis;
        m_ptsModel.beginModelChange();
    }else if (m_SimStatus == INACTIVE){

        qreal angle_rad = qDegreesToRadians(angle.toReal());
        setRotationAngle( angle_rad, ROTATION_AXIS(axis.toInt()));
        qDebug() << "(receivePlay) START => Angle   : " << angle.toInt() << " degrees";
        qDebug() << "(receivePlay) START => Angle   : " << angle_rad << " radians.";
        qDebug() << "(receivePlay) START => Axis    : " << axis.toInt();
        qDebug() << "(receivePlay) START => Cycles  : " << num_cycles.toInt();
        qDebug() << "(receivePlay) START => Threads : " << QThread::idealThreadCount();

         QGenericMatrix<3,3,qreal> transform;

        switch (m_RotationAxis) {
        case X_AXIS:
            transform = m_xAxisRotation;
            break;
        case Y_AXIS:
            transform = m_yAxisRotation;
            break;
        case Z_AXIS:
            transform = m_zAxisRotation;
            break;
        case ALL_AXES:
            transform = m_xAxisRotation*m_yAxisRotation*m_zAxisRotation;
            break;
        }

        auto rotationSim = [angle, transform, num_cycles] (SpatialPoint& _pt){
            volatile qreal accumAngle = 0.0;
            while( int(accumAngle/360.0) < num_cycles.toInt() ){
                accumAngle += angle.toReal();
                _pt.transformPoint(transform);
                qDebug() << "(Pt: " << _pt.getID() << ") accumulated angle => " << accumAngle
                         << " current num cycles => " << accumAngle/360.0;
                //QThread::msleep(1);
            }
        };

        m_FutureWatcher.setFuture( QtConcurrent::map(m_ptsModel.m_SpatialPts, rotationSim) );

        m_ptsModel.beginModelChange();
    }else{

    }
}

void SpatialPointManager::receivePause()
{
    m_FutureWatcher.pause();
    m_SimStatus = PAUSED;
    qDebug() << "(receivePlay) PAUSE => ";
    m_ptsModel.endModelChange();
    updateSimStatus(0, QVariant(m_SimProgress), m_SimStatus);
}

void SpatialPointManager::receiveCancel()
{
    m_FutureWatcher.cancel();
    m_SimStatus = CANCELLED;
    qDebug() << "(receiveCancel) CANCELLED => ";
    m_ptsModel.endModelChange();
    updateSimStatus(0, QVariant(m_SimProgress), m_SimStatus);
}

void SpatialPointManager::onFinished()
{
    m_ptsModel.endModelChange();
    m_SimStatus = INACTIVE;
    qDebug() << "(onFinished) INACTIVE => ";
    updateSimStatus(0, QVariant(m_SimProgress), m_SimStatus);
}

void SpatialPointManager::onProgressValueChanged(int val)
{
    m_ptsModel.endModelChange();
    m_SimStatus = RUNNING;
    m_SimProgress = qreal(val) /(m_SimProgressMax - m_SimProgressMin);
    qDebug() << "(onProgressValueChanged) RUNNING => ";
    updateSimStatus(QThread::idealThreadCount(), QVariant(m_SimProgress), m_SimStatus);
    m_ptsModel.beginModelChange();
}

void SpatialPointManager::onProgressRangeChanged(int min, int max)
{
    m_SimProgressMin = qreal(min);
    m_SimProgressMax = qreal(max);
    qDebug() << "(onProgressRangeChanged) Min: " << min << " Max: " << max;
}

void SpatialPointManager::clearModel()
{
    m_ptsModel.beginModelChange();
    m_ptsModel.m_SpatialPts.resize(0);
    m_ptsModel.endModelChange();
}

/*
 * This function performs the transformation of the residing set of points according to the
 * member transform matrices, which are presumed to have been set prior to this function being
 * called).
*/
void SpatialPointManager::transformModel()
{
        m_ptsModel.beginModelChange();

        for ( int i = 0; i < m_ptsModel.m_SpatialPts.size(); i++ ){
            transformPt(m_ptsModel.m_SpatialPts[i]);
        }

        m_ptsModel.endModelChange();

}

void SpatialPointManager::transformPt(SpatialPoint &pt)
{
    switch (m_RotationAxis) {
    case X_AXIS:
        pt.transformPoint(m_xAxisRotation);
        break;
    case Y_AXIS:
        pt.transformPoint(m_yAxisRotation);
        break;
    case Z_AXIS:
        pt.transformPoint(m_zAxisRotation);
        break;
    case ALL_AXES:
        pt.transformPoint(m_xAxisRotation);
        pt.transformPoint(m_yAxisRotation);
        pt.transformPoint(m_zAxisRotation);
        break;
    }
}

/*
 * This function is to be used for setting the transformation matrices that will be applied
 * to the residing point set when tranformModel is called. The transformation matrices for rotation
 * about each axis can either be individually set by calling this function with the corresponding axis
 * specified or the same angle can be applied to all the axes with one call specifiying the axis parameter
 * as "ALL_AXES".
*/
void SpatialPointManager::setRotationAngle(const qreal &angle, const ROTATION_AXIS &axis)
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

QVariant SpatialPointManager::getRotationAngle(const ROTATION_AXIS &axis)
{
    switch (axis) {
    case X_AXIS:
        return m_xAngle;
    case Y_AXIS:
        return m_yAngle;
    case Z_AXIS:
        return m_zAngle;
    default:
        return m_xAngle;
    }
}

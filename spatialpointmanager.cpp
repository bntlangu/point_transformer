#include <QtMath>
#include "spatialpointmanager.h"

SpatialPointManager::SpatialPointManager(QObject *parent) : QObject(parent)
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
}

void SpatialPointManager::createLine(const QVariant &num_pts, const QVariant &inter_space, const QVariant &shape_idx)
{
    m_ptsModel.clearModel();

    float x = 0, y = 0, z = 0;

    for (int i = 0; i < num_pts.toInt(); i++){
        m_ptsModel.addPoint(x, y, z);

        if(shape_idx == HORIZONTAL_LINE){
            x += inter_space.toInt();
        }else if (shape_idx == VERTICAL_LINE){
            y += inter_space.toInt();
        }
    }
}

// Here we make the assumption that the input value coming in from
// the UI is on the range [0,16777216] i.e. 24-bit color (2^24)
void SpatialPointManager::applyColorTransform(const QVariant &colorVal)
{
    uint32_t uColorVal = colorVal.toUInt()*10;

    // Convert integer value into hex
    QString strColor = QString("#%1").arg(uColorVal, 6, 16, QLatin1Char( '0' ));

    // Push the hex string back to the UI
    emit updateColor(strColor);
}

// Here we make the assumption that the input value coming in from
// the UI is on the range [0,1]
void SpatialPointManager::applySizeTransform(const QVariant &sizeVal)
{
    float fSize = sizeVal.toFloat();

    // Push the hex string back to the UI
    emit updateSize(fSize);
}

// Here we make the assumption that the input value coming in from
// the UI is on the range [0,360] i.e. angle in degrees.
void SpatialPointManager::applyRotationTransform(const QVariant &rotationVal)
{
    // Convert integer value to angle (radians)
    float fRotationAngle = (rotationVal.toFloat() / float(360.0)) * float(2.0*M_PI);

    // Apply the requisite transform on each point

}

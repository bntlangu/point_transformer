#ifndef SPATIALPOINTMANAGER_H
#define SPATIALPOINTMANAGER_H

#include <QObject>
#include <QVector>

#include <spatialpointmodel.h>

enum SHAPE_TYPE{
    HORIZONTAL_LINE = 0,
    VERTICAL_LINE
};
Q_DECLARE_METATYPE(SHAPE_TYPE)

enum ROTATION_AXIS{
    X_AXIS = 0,
    Y_AXIS,
    Z_AXIS,
    ALL_AXES

};
Q_DECLARE_METATYPE(ROTATION_AXIS)

class SpatialPointManager : public QObject
{
    Q_OBJECT
public:
    explicit SpatialPointManager(QObject *parent = nullptr);

    SpatialPointModel m_ptsModel;


signals:

    void updateColor(const QVariant &colorVal);

    void updateSize(const QVariant &sizeVal);

    // This shouldn't be necessary since the model should be automatically updated when changed.
    void updatePts();

public slots:

    void createLine(const QVariant &num_pts, const QVariant &inter_space, const QVariant &shape_idx);

    // This function takes an input value from the UI "color" slider and converts it to a
    // hex color string, which in turn is used to set the base color of the points.
    void applyColorTransform(const QVariant &colorVal);

    // This function takes an input value from the UI "size" slider and converts it to a
    // size value, which in turn is used to set the size of the points on the plot.
    void applySizeTransform(const QVariant& sizeVal);

    // This function takes an input value from the UI "rotation" slider and converts it to an
    // angle value, which in turn is used to set X, Y, and Z coordinates of each point in the model.
    void applyRotationTransform(const QVariant& rotationVal, const QVariant &rotationAxis);

private:

    void performRotation(QVector<SpatialPoint> &pts, const QGenericMatrix<3,3,qreal> &transformMtx);

    void setRotationAngle(const qreal &angle, const ROTATION_AXIS &axis);

    QGenericMatrix<3,3,qreal> m_xAxisRotation;
    QGenericMatrix<3,3,qreal> m_yAxisRotation;
    QGenericMatrix<3,3,qreal> m_zAxisRotation;

};

#endif // SPATIALPOINTMANAGER_H

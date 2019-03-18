#ifndef SPATIALPOINTMANAGER_H
#define SPATIALPOINTMANAGER_H

#include <QObject>
#include <QVector>
#include <QtConcurrent/QtConcurrent>

#include <functional>

#include <spatialpointmodel.h>

enum SHAPE_TYPE{
    HORIZONTAL_LINE = 0,
    VERTICAL_LINE
};
Q_DECLARE_METATYPE(SHAPE_TYPE)

enum SIM_STATUS{
    INACTIVE = 0,
    RUNNING,
    PAUSED,
    FINISHED,
    CANCELLED
};
Q_DECLARE_METATYPE(SIM_STATUS)

class SpatialPointManager : public QObject
{
    Q_OBJECT
public:
    explicit SpatialPointManager(QObject *parent = nullptr);

    SpatialPointModel m_ptsModel;

    void clearModel();

    void setRotationAngle(const qreal &angle, const ROTATION_AXIS &axis);

    QVariant getRotationAngle(const ROTATION_AXIS &axis);

    void transformModel();

    void transformPt(SpatialPoint &pt);

signals:

    void updateColor(const QVariant &colorVal);

    void updateSize(const QVariant &sizeVal);

    // This shouldn't be necessary since the model should be automatically updated when changed.
    //void updatePts();

    void updateSimStatus(const QVariant &numThreads, const QVariant &simProgress, const QVariant &simStatus);

    //void forwardCancel();

public slots:

    // Slots for the UI to connect to
    void createLine(const QVariant &num_pts, const QVariant &origin, const QVariant &shape_idx);

    void createRandomPts(const QVariant &num_pts, const QVariant &max_val);

    // This function takes an input value from the UI "color" slider and converts it to a
    // hex color string, which in turn is used to set the base color of the points.
    void applyColorTransform(const QVariant &colorVal);

    // This function takes an input value from the UI "size" slider and converts it to a
    // size value, which in turn is used to set the size of the points on the plot.
    void applySizeTransform(const QVariant& sizeVal);

    // This function takes an input value from the UI "rotation" slider and converts it to an
    // angle value, which in turn is used to set X, Y, and Z coordinates of each point in the model.
    void applyRotationTransform(const QVariant& angle, const QVariant &axis);

    // Slots for the automated transform simulation controls
    void receivePlay(const QVariant &angle, const QVariant &axis, const QVariant &num_cycles);

    void receivePause();

    void receiveCancel();

    // Slots for the future watcher to connect to
    void onFinished();// <TODO> Reset the progressBar

    void onProgressValueChanged(int val);// <TODO> Update the value of the progressBar

    void onProgressRangeChanged(int min, int max);// <TODO> Update the range of the progressBar



private:

    QGenericMatrix<3,3,qreal> m_xAxisRotation;
    QGenericMatrix<3,3,qreal> m_yAxisRotation;
    QGenericMatrix<3,3,qreal> m_zAxisRotation;

    qreal m_xAngle;
    qreal m_yAngle;
    qreal m_zAngle;

    qreal m_SimProgress;
    qreal m_SimProgressMin;
    qreal m_SimProgressMax;

    ROTATION_AXIS m_RotationAxis;
    SIM_STATUS m_SimStatus;

    QFutureWatcher<void> m_FutureWatcher;

    bool m_Paused;

};

#endif // SPATIALPOINTMANAGER_H

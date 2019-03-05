#ifndef SPATIALPOINTMANAGER_H
#define SPATIALPOINTMANAGER_H

#include <QObject>
#include <spatialpointmodel.h>

enum SHAPE_TYPE{
    HORIZONTAL_LINE = 0,
    VERTICAL_LINE
};
Q_DECLARE_METATYPE(SHAPE_TYPE)

class SpatialPointManager : public QObject
{
    Q_OBJECT
public:
    explicit SpatialPointManager(QObject *parent = nullptr);

    SpatialPointModel m_ptsModel;

signals:

public slots:

    void createLine(const QVariant &num_pts, const QVariant &inter_space, const QVariant &shape_idx);


private:

};

#endif // SPATIALPOINTMANAGER_H

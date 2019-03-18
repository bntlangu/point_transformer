#ifndef SPATIALPOINTMODEL_H
#define SPATIALPOINTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QGenericMatrix>
#include <QThread>
#include <QtMath>

enum ROTATION_AXIS{
    X_AXIS = 0,
    Y_AXIS,
    Z_AXIS,
    ALL_AXES

};
Q_DECLARE_METATYPE(ROTATION_AXIS)


struct SpatialPoint
{
public:
    SpatialPoint();
//        { m_point(0,0) = 0.0; m_point(0,1) = 0.0; m_point(0,2) = 0.0; }

    SpatialPoint(const qreal &X, const qreal &Y, const qreal &Z);
//        { m_point(0,0) = X; m_point(0,1) = Y; m_point(0,2) = Z; }

    int getID(){ return m_ID; }

    float X() const { return float(m_point(0,0)); }
    float Y() const { return float(m_point(0,1)); }
    float Z() const { return float(m_point(0,2)); }

    void transformPoint(const QGenericMatrix<3,3,qreal> &transformMtx)
        { m_point = m_point*transformMtx; } // We have assumed here that a 3x3 mtx multiplied by a 3x1 mtx results in a 3x1
                                            // verify that the reult produced is indeed consistent with the expectd answer.
private:
    QGenericMatrix<3,1,qreal> m_point;

    int m_ID;
};


class SpatialPointModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SpatialPointModel(QObject *parent = nullptr);

    enum ModelRoles{
        IDRole = Qt::UserRole + 1,
        XRole,
        YRole,
        ZRole,
        AngleRole
    };

    void addPoint( const QVariant &X,
                   const QVariant &Y,
                   const QVariant &Z
                  );

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

    void beginModelChange(){beginResetModel();}

    void endModelChange(){endResetModel();}

    QVector<SpatialPoint> m_SpatialPts;

signals:

public slots:

protected:
    QHash<int,QByteArray> roleNames() const;

private:

};

#endif // SPATIALPOINTMODEL_H

#ifndef SPATIALPOINTMODEL_H
#define SPATIALPOINTMODEL_H

#include <QObject>
#include <QAbstractListModel>

struct SpatialPoint
{
public:
    SpatialPoint(const float &X,
                 const float &Y,
                 const float &Z
                 )
        : m_X(X),
          m_Y(Y),
          m_Z(Z)
    {}

    float X() const {return m_X;}
    float Y() const {return m_Y;}
    float Z() const {return m_Z;}

private:
    float m_X;
    float m_Y;
    float m_Z;
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
        ZRole
    };

    void addPoint( const QVariant &X,
                   const QVariant &Y,
                   const QVariant &Z
                  );

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;


signals:

public slots:

protected:
    QHash<int,QByteArray> roleNames() const;

private:

    QList<SpatialPoint> m_SpatialPts;
};

//Q_DECLARE_METATYPE(SpatialPointModel)

#endif // SPATIALPOINTMODEL_H

#ifndef SPATIALPOINTMANAGER_H
#define SPATIALPOINTMANAGER_H

#include <QObject>

class SpatialPointManager : public QObject
{
    Q_OBJECT
public:
    explicit SpatialPointManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SPATIALPOINTMANAGER_H
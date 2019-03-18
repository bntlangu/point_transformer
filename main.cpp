#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>

#include "spatialpointmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    SpatialPointManager ptsMgr;

    // Set the model for the view context
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext* context = view.rootContext();
    context->setContextProperty("ptsModel", &ptsMgr.m_ptsModel);

    // Grab specific components from the QML, which we want to listen/connect to
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    QObject* rootItem = view.rootObject();// implicit cast from "QQuickItem" is happening here

    // Connect the signals from the shape creation controls to the point manager
    QObject::connect(
        rootItem, SIGNAL(sendCreateLine(QVariant, QVariant, QVariant)),
        &ptsMgr, SLOT(createLine(QVariant, QVariant, QVariant))
    );

    // Connect the signals from the shape creation controls to the point manager
    QObject::connect(
        rootItem, SIGNAL(sendGeneratePts(QVariant, QVariant)),
        &ptsMgr, SLOT(createRandomPts(QVariant, QVariant))
    );

    // Connect the signals from the MANUAL point attribute controls to the point manager
    QObject::connect(
        rootItem, SIGNAL(requestSizeTransform(QVariant)),
        &ptsMgr, SLOT(applySizeTransform(QVariant))
    );

    QObject::connect(
        rootItem, SIGNAL(requestColorTransform(QVariant)),
        &ptsMgr, SLOT(applyColorTransform(QVariant))
    );

    QObject::connect(
        rootItem, SIGNAL(requestRotationTransform(QVariant, QVariant)),
        &ptsMgr, SLOT(applyRotationTransform(QVariant, QVariant))
    );

    // Connect the signals from the AUTO point attribute controls to the point manager
    QObject::connect(
        rootItem, SIGNAL(sendPlay(QVariant, QVariant, QVariant)),
        &ptsMgr, SLOT(receivePlay(QVariant, QVariant, QVariant))
    );

    QObject::connect(
        rootItem, SIGNAL(sendPause()),
        &ptsMgr, SLOT(receivePause())
    );

    QObject::connect(
        rootItem, SIGNAL(sendCancel()),
        &ptsMgr, SLOT(receiveCancel())
    );

    // Connect the signals from the point manager to the data view
    QObject::connect(
        &ptsMgr, SIGNAL(updateSize(QVariant)),
        rootItem, SLOT(onApplySizeUpdate(QVariant))
    );

    QObject::connect(
        &ptsMgr, SIGNAL(updateColor(QVariant)),
        rootItem, SLOT(onApplyColorUpdate(QVariant))
    );

    QObject::connect(
        &ptsMgr, SIGNAL(updateSimStatus(QVariant, QVariant, QVariant)),
        rootItem, SLOT(onUpdateSimStatus(QVariant, QVariant, QVariant))
    );

    return app.exec();
}

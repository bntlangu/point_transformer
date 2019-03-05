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

    //QObject* pThreadInput = rootItem->findChild<QObject*>("workCreator");

    // Connect the workermgr to app's signals
    QObject::connect(
        rootItem, SIGNAL(sendCreateLine(QVariant, QVariant, QVariant)),
        &ptsMgr, SLOT(createLine(QVariant, QVariant, QVariant))
    );

    // <TODO>
    // Provide slots for the signals generated from the controls on the UI
    // to create the corresponding list of points
    // Generate the list of points
    // Plot the list of points using scatter 3d


    return app.exec();
}

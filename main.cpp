#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "spatialpointmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // Create model
    SpatialPointModel ptsModel;

    // Populate with dummy data
    ptsModel.addPoint(-10.0, 5.0, -5.0);
    ptsModel.addPoint(-9.0, 3.0, -4.5);
    ptsModel.addPoint(-8.5, 4.1, -4.0);
    ptsModel.addPoint(-8.0, 4.75, -3.9);
    ptsModel.addPoint(-9.5, 4.9, -4.2);
    ptsModel.addPoint(-9.9, 3.42, -3.5);
    ptsModel.addPoint(-7.8, 3.1, -4.9);
    ptsModel.addPoint(-7.3, 2.91, -4.1);
    ptsModel.addPoint(-7.1, 3.68, -4.52);
    ptsModel.addPoint(-8.8, 2.96, -3.6);
    ptsModel.addPoint(-6.94, 2.4, -2.92);
    ptsModel.addPoint(-9.02, 4.74, -4.18);
    ptsModel.addPoint(-9.54, 3.1, -3.8);
    ptsModel.addPoint(-6.86, 3.66, -3.58);
    ptsModel.addPoint(-8.16, 1.82, -4.64);
    ptsModel.addPoint(-7.4, 3.18, -4.22);
    ptsModel.addPoint(-7.9, 3.06, -4.3);
    ptsModel.addPoint(-8.98, 2.64, -4.44);
    ptsModel.addPoint(-6.36, 3.96, -4.38);
    ptsModel.addPoint(-7.18, 3.32, -4.04);
    ptsModel.addPoint(-7.9, 3.4, -2.78);
    ptsModel.addPoint(-7.4, 3.12, -3.1);
    ptsModel.addPoint(-7.54, 2.8, -3.68);

    // Set the model for the view context
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext* context = view.rootContext();
    context->setContextProperty("ptsModel", &ptsModel);

    // Grab specific components from the QML, which we want to listen/connect to
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    // <TODO>
    // Provide slots for the signals generated from the controls on the UI
    // to create the corresponding list of points
    // Generate the list of points
    // Plot the list of points using scatter 3d


    return app.exec();
}

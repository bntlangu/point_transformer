#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // <TODO>
    // Provide slots for the signals generated from the controls on the UI
    // to create the corresponding list of points
    // Generate the list of points
    // Plot the list of points using scatter 3d


    return app.exec();
}

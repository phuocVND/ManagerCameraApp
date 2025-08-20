#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "udpcamera.h"

#include <QQuickImageProvider>
#include <QPixmap>
#include <QMutex>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    UDPCamera* udpCam = new UDPCamera("0.0.0.0", 8888);

    udpCam->open();
    udpCam->captureLoop();

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("CameraApp", "Main");

    return app.exec();
}

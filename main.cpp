#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "speedometer.h"
#include <QTimer>
#include <unistd.h>
#include <QQmlContext>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    /* Register a C++ type, so com.luyaohan1001.speedometer can be imported to QML as well. */
    qmlRegisterType<Speedometer>("com.luyaohan1001.speedometer", 1, 0, "Speedometer");

    QQmlApplicationEngine engine;

    /* set contect property allows speedometer instance to be accessable in QML objects as well. */
    Speedometer *speedometer = new Speedometer();
    engine.rootContext()->setContextProperty("speedometer", speedometer);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);



    return app.exec();
}

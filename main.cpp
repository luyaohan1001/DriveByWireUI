#include <unistd.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include "blinkers.h"
#include "brakes.h"
#include "speedometer.h"
#include "steeringwheel.h"
#include "udp_controller.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);

  /* Register a C++ type, so com.luyaohan1001.speedometer can be imported to QML
   * as well. */
  qmlRegisterType<Speedometer>("com.luyaohan1001.speedometer", 1, 0,
                               "Speedometer");

  /* Initantialize QQmlApplicationEngine. */
  QQmlApplicationEngine engine;

  Brakes *brakes = new Brakes();
  Blinkers *blinkers = new Blinkers();
  Speedometer *speedometer = new Speedometer();
  SteeringWheel *steeringWheel = new SteeringWheel();
  UDP_Controller *udpController = new UDP_Controller();

  udpController->blinkers = blinkers;
  udpController->speedometer = speedometer;
  udpController->brakes = brakes;
  udpController->steeringWheel = steeringWheel;

  /* set context property to allow C++ object instance properties to be
   * accessable from QML objects. */
  engine.rootContext()->setContextProperty("speedometer", speedometer);
  engine.rootContext()->setContextProperty("blinkers", blinkers);
  engine.rootContext()->setContextProperty("brakes", brakes);
  engine.rootContext()->setContextProperty("steeringWheel", steeringWheel);

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}

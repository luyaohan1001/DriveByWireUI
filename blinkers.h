#ifndef BLINKERS_H
#define BLINKERS_H

#include <QObject>
#include <QTimer>

#define BLINKER_ON true
#define BLINKER_OFF false

#define PRESSED true
#define RELEASED false

class Blinkers : public QObject {
  Q_OBJECT
  /* Declare properties that can be assessed by QML elements. */
  Q_PROPERTY(bool leftBlinkerState READ getLeftBlinkerState WRITE
                 setLeftBlinkerState NOTIFY leftBlinkerStateChangedSignal);
  Q_PROPERTY(bool rightBlinkerState READ getRightBlinkerState WRITE
                 setRightBlinkerState NOTIFY rightBlinkerStateChangedSignal);
  Q_PROPERTY(
      bool emergentBlinkerState READ getEmergentBlinkerState WRITE
          setEmergentBlinkerState NOTIFY emergentBlinkerStateChangedSignal);

 public:
  /* Constructor */
  explicit Blinkers(QObject *parent = nullptr);

  /* Setters. */
  void setLeftBlinkerState(bool state);
  void setRightBlinkerState(bool state);
  void setEmergentBlinkerState(bool state);

  /* Getters. */
  bool getLeftBlinkerState();
  bool getRightBlinkerState();
  bool getEmergentBlinkerState();

  QTimer *leftBlinkerTimer;
  QTimer *rightBlinkerTimer;
  QTimer *emergentBlinkerTimer;
 signals:
  void leftBlinkerStateChangedSignal();
  void rightBlinkerStateChangedSignal();
  void emergentBlinkerStateChangedSignal();

 public slots:
  void leftBlinkerBlinks();
  void rightBlinkerBlinks();
  void emergentBlinkerBlinks();

 private:
  bool leftBlinkerState;
  bool rightBlinkerState;
  bool emergentBlinkerState;

  const static int BLINKER_PULSE_DELAY = 300;
};

#endif  // BLINKERS_H

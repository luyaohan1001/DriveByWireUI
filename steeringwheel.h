#ifndef STEERINGWHEEL_H
#define STEERINGWHEEL_H

#include <QObject>

class SteeringWheel : public QObject {
  Q_OBJECT
  /* Declare properties that can be assessed by QML elements. */
  Q_PROPERTY(qreal steeringPosition READ getSteeringPosition WRITE
                 setSteeringPosition NOTIFY steeringPositionChangedSignal);

 public:
  /* Constructor. */
  explicit SteeringWheel(QObject *parent = nullptr);

  /* Setters. */
  void setSteeringPosition(qreal s);

  /* Getters. */
  qreal getSteeringPosition();

 signals:
  void steeringPositionChangedSignal();

 private:
  qreal steeringPosition;
};

#endif  // STEERINGWHEEL_H

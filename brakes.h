#ifndef BRAKES_H
#define BRAKES_H

#include <QObject>

#define BRAKE_PRESSED true
#define BRAKE_RELEASED false

class Brakes : public QObject {
  Q_OBJECT
  /* Declare properties that can be assessed by QML elements. */
  Q_PROPERTY(qreal brakesState READ getBrakesState WRITE setBrakesState NOTIFY
                 brakesChangedSignal);

 public:
  /* Constructor. */
  explicit Brakes(QObject *parent = nullptr);

  /* Setters. */
  void setBrakesState(bool s);

  /* Getters. */
  bool getBrakesState();

 signals:
  void brakesChangedSignal();

 private:
  bool brakesState;

 signals:
};

#endif  // BRAKES_H

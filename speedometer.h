#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include <QObject>

/**
 * @brief The Speedometer class
 * @note This class serves as backend controller for the speed meter element on a car cockpit dashboard.
 */
class Speedometer : public QObject
{

    Q_OBJECT
    /* Declare properties that can be assessed by QML elements. */
    Q_PROPERTY(qreal speedValue READ getSpeed WRITE setSpeed NOTIFY speedChangedSignal);

public:
    /* Constructor. */
    explicit Speedometer(QObject *parent = 0);

    /* Setters. */
    void setSpeed(qreal s);

    /* Getters. */
    qreal getSpeed();

signals:
    void speedChangedSignal();

private:
    qreal currSpeed;

};

#endif // SPEEDOMETER_H

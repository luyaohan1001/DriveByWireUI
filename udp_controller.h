#ifndef UDP_CONTROLLER_H
#define UDP_CONTROLLER_H

#include <QUdpSocket>
#include <QObject>
#include "speedometer.h"

class UDP_Controller : public QObject
{
public:
    explicit UDP_Controller(QObject *parent = 0);
    ~UDP_Controller();
    Speedometer *speedometer;

public slots:
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;


};

#endif // UDP_CONTROLLER_H

#include "udp_controller.h"

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDebug>

/**
 * @brief UDP_Controller::UDP_Controller
 * @param parent
 */
UDP_Controller::UDP_Controller(QObject *parent)
    : QObject(parent)
{
    udpSocket = new QUdpSocket();
    udpSocket->bind(QHostAddress("192.168.137.74"), 8000);

    connect(udpSocket, &QUdpSocket::readyRead, this, &UDP_Controller::readPendingDatagrams);
    qDebug() << "UDP socket connected. ";
}

/**
 * @brief UDP_Controller::~UDP_Controller
 */
UDP_Controller::~UDP_Controller() {
    delete udpSocket;
}


void processDatagram() {
    /*
     * Example datagram:
     * "71030000
     *  33fcffff
     *  ff7f0000
     *  00000000
     *  00000000
     *  00000000
     *  ff7f0000
     *  ff7f0000
     *  00000000
     *  ffffffff
     *  ffffffff
     *  ffffffff
     *  ffffffff
     *  00000000
     *  00000000
     *  00000000
     *  00000000
     *  00000000
     *  ........
     *  ........
     *  16 byte steer
     *
     */

}


/**
 * @brief UDP_Controller::readPendingDatagrams
 */
void UDP_Controller::readPendingDatagrams()
{

    qDebug() << "UDP Datagram signal triggered. ";
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        qDebug() << ((QByteArray)(datagram.data().toHex()));
        QByteArray rawData = ((QByteArray)(datagram.data()));
        // qDebug() << rawData.mid(0, 16);
        // QByteArray steerPositionArray = rawData.mid(0, 16);
        int32_t steerPosition =  (int32_t)((rawData[5] << 8) + rawData[6]);
        int32_t throttlePosition =  (int32_t)((rawData[9] << 8) + rawData[10]);
        int32_t normalizedThrottlePosition = (int32_t)((((double)(-1 * throttlePosition)) + 32768.0) / 65535.0 * 100.0);
        this->speedometer->setSpeed(normalizedThrottlePosition);
        qDebug() << normalizedThrottlePosition;
    }
}

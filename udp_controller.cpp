#include "udp_controller.h"

#include <QDebug>
#include <QNetworkDatagram>
#include <QUdpSocket>

/**
 * @brief UDP_Controller::UDP_Controller Constructor
 * @param parent
 */
UDP_Controller::UDP_Controller(QObject *parent) : QObject(parent) {
  udpSocket = new QUdpSocket();
  udpSocket->bind(QHostAddress("192.168.137.74"), 8002);

  /* Connect UDP read ready signal to signal callback. */
  connect(udpSocket, &QUdpSocket::readyRead, this,
          &UDP_Controller::udpDatagramReceiveCallback);
  qDebug() << "UDP socket connected. ";
}

/**
 * @brief UDP_Controller::~UDP_Controller
 */
UDP_Controller::~UDP_Controller() { delete udpSocket; }

void UDP_Controller::brakesLogic() {
  if (normalizedBrakePosition > 7) {
    this->brakes->setBrakesState(true);
  } else {
    this->brakes->setBrakesState(false);
  }
}

void UDP_Controller::steeringTurnActionSenseLogic() {
  /* RIGHT-TURN overwrites the LEFT blinker state. */
  if (turnState == TURN_ACTION_INCOMPLETE &&
      blinkers->getLeftBlinkerState() == ON &&
      ((turnStartPosition - receivedSteerPosition) >
       TURN_ACTION_SENSE_THRESHOLD)) {
    turnState = PASSED_TURN_SENSE_THRESHOLD;
    /* Left-TURN overwrites the RIGHT blinker state. */
  } else if (turnState == TURN_ACTION_INCOMPLETE &&
             blinkers->getRightBlinkerState() == ON &&
             ((turnStartPosition - receivedSteerPosition) <
              -TURN_ACTION_SENSE_THRESHOLD)) {
    turnState = PASSED_TURN_SENSE_THRESHOLD;
  } else if (turnState == PASSED_TURN_SENSE_THRESHOLD &&
             blinkers->getLeftBlinkerState() == ON &&
             (-(receivedSteerPosition - TURN_ACTION_SENSE_THRESHOLD) >
              TURN_ACTION_CANCEL_THRESHOLD)) {
    turnState = TURN_ACTION_COMPLETED;
    this->blinkers->setLeftBlinkerState(OFF);
  } else if (turnState == PASSED_TURN_SENSE_THRESHOLD &&
             blinkers->getRightBlinkerState() == ON &&
             ((TURN_ACTION_SENSE_THRESHOLD - receivedSteerPosition) >
              TURN_ACTION_CANCEL_THRESHOLD)) {
    turnState = TURN_ACTION_COMPLETED;
    this->blinkers->setRightBlinkerState(OFF);
  }
}

void UDP_Controller::latchPressToBlinkerStates() {
    bool leftBlinkerPressed = (receivedDataPacket.rgbButtons[9] == 0) ? false : true;
    bool rightBlinkerPressed = (receivedDataPacket.rgbButtons[8] == 0) ? false : true;
    bool emergentBlinkerPressed = (receivedDataPacket.rgbButtons[7] == 0) ? false : true;

    /* EMERGENT Blinker PRESSED. LEFT/RIGHT Blinker don't care. */
    if (emergentBlinkerPressed == true) {
        qDebug() << "emergent Blinker pressed";
        this->blinkers->setEmergentBlinkerState(ON);

    /* LEFT Blinker PRESSED, RIGHT Blinker NOT PRESSED */
    } else if (leftBlinkerPressed == true
        && rightBlinkerPressed == false) {
        this->blinkers->setLeftBlinkerState(ON);
        qDebug() << "left Blinker pressed";

        /* Record initial steer position when blinker is PRESSED. */
        turnStartPosition = receivedSteerPosition;

        /* Initialize state machine for steer-wheel-cancel-turn-blinker logic. */
        turnState = TURN_ACTION_INCOMPLETE;
    /* RIGHT Blinker PRESSED, LEFT Blinker NOT PRESSED */
    } else if (rightBlinkerPressed == true
        && leftBlinkerPressed == false) {
        qDebug() << "right Blinker pressed";
        this->blinkers->setRightBlinkerState(ON);
        /* Record initial steer position when blinker is PRESSED. */
        turnStartPosition = receivedSteerPosition;

        /* Initialize state machine for steer-wheel-cancel-turn-blinker logic. */
        turnState = TURN_ACTION_INCOMPLETE;
    }

}

void UDP_Controller::blinkersControlLogic() {
    latchPressToBlinkerStates();
    steeringTurnActionSenseLogic();
}

void UDP_Controller::speedometerLogic() {
  /* Set received speed data to speedometer object. */
  if (this->brakes->getBrakesState() == BRAKE_PRESSED) {
    this->speedometer->setSpeed(0);
  } else {
    this->speedometer->setSpeed(normalizedThrottlePosition);
  }
}

void UDP_Controller::steeringWheelLogic() {
  /* Set received speed data to speedometer object. */
  this->steeringWheel->setSteeringPosition(normalizedSteeringPosition);
}

/**
 * @brief UDP_Controller::udpDatagramReceiveCallback
 */
void UDP_Controller::udpDatagramReceiveCallback() {


  qDebug() << "UDP Datagram signal triggered. ";
  while (udpSocket->hasPendingDatagrams()) {
    uint8_t rawData[4 + sizeof(receivedDataPacket)];

    /* Receive UDP datagram. */
    QNetworkDatagram datagram = udpSocket->receiveDatagram();

    /* Convert datagram to a byte array. */
    memcpy(rawData, datagram.data(), sizeof(rawData));

    /* Load received raw data to known Logitech packet format to access
     * individual fields (throttle, brake, turn signals...) */
    memcpy(&receivedDataPacket, ((uint8_t *)&rawData) + 4,
           sizeof(receivedDataPacket));

    /* Normalize raw value to scale. */
    normalizedThrottlePosition =
        (int32_t)((((double)(-1 * (int16_t)receivedDataPacket.lY)) + 32768.0) /
                  65535.0 * 100.0);
    receivedSteerPosition = (int16_t)receivedDataPacket.lX;
    normalizedSteeringPosition =
        (int32_t)((((double)((int16_t)receivedDataPacket.lX))) / 65535.0 *
                  900.0);
    normalizedBrakePosition =
        (int32_t)((((double)(-1 * (int16_t)receivedDataPacket.lRz)) + 32768.0) /
                  65535.0 * 100.0);

    /* Run each components logic. */
    blinkersControlLogic();
    brakesLogic();
    speedometerLogic();
    qDebug() << normalizedSteeringPosition;
    steeringWheelLogic();
  }
}

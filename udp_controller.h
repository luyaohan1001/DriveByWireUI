#ifndef UDP_CONTROLLER_H
#define UDP_CONTROLLER_H

#include <QObject>
#include <QUdpSocket>

#include "blinkers.h"
#include "brakes.h"
#include "speedometer.h"
#include "steeringwheel.h"

/* Data packet received from the Logitech Steering Wheel. */
typedef struct {
  int lX;
  int lY;
  int lZ;
  int lRx;
  int lRy;
  int lRz;
  int rglSlider[2];
  unsigned rgdwPOV[4];
  char rgbButtons[128];
  int lVX;
  int lVY;
  int lVZ;
  int lVRx;
  int lVRy;
  int lVRz;
  int rglVSlider[2];
  int lAX;
  int lAY;
  int lAZ;
  int lARx;
  int lARy;
  int lARz;
  int rglASlider[2];
  int lFX;
  int lFY;
  int lFZ;
  int lFRx;
  int lFRy;
  int lFRz;
  int rglFSlider[2];
} LogitechSteeringWheelPacket_t;

/* Blinker States. */
typedef enum { OFF = false, ON = true } eBlinker_t;

/* Union Type used to record the blinker states. */
class UDP_Controller : public QObject {
  typedef enum {
    TURN_ACTION_INCOMPLETE,
    PASSED_TURN_SENSE_THRESHOLD,
    TURN_ACTION_COMPLETED
  } eTurnState_t;

 public:
  explicit UDP_Controller(QObject *parent = 0);
  ~UDP_Controller();
  void emulatePackets();
  Brakes *brakes;
  Blinkers *blinkers;
  Speedometer *speedometer;
  SteeringWheel *steeringWheel;

 public slots:
  void steeringTurnActionSenseLogic();
  void blinkersControlLogic();
  void brakesLogic();
  void speedometerLogic();
  void steeringWheelLogic();
  void latchPressToBlinkerStates();

  void udpDatagramReceiveCallback();

 private:
  QUdpSocket *udpSocket;
  eTurnState_t turnState;
  int32_t turnStartPosition;
  LogitechSteeringWheelPacket_t receivedDataPacket;
  int16_t receivedSteerPosition;

  int32_t normalizedThrottlePosition;
  int32_t normalizedSteeringPosition;
  int32_t normalizedBrakePosition;
  const int16_t TURN_ACTION_SENSE_THRESHOLD =
      6000;  // When the driver right turns on the LEFT blinker, it should
             // overwrite the turn off the left blinker.
  const int16_t TURN_ACTION_CANCEL_THRESHOLD =
      1000;  // When the driver right turns on the LEFT blinker, it should
             // overwrite the turn off the left blinker.
};

#endif  // UDP_CONTROLLER_H

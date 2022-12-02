#include "blinkers.h"

#include <QDebug>
#include <QTimer>

Blinkers::Blinkers(QObject *parent)
    : QObject{parent}

{
  this->leftBlinkerState = BLINKER_OFF;
  this->rightBlinkerState = BLINKER_OFF;
  this->emergentBlinkerState = BLINKER_OFF;

  this->leftBlinkerTimer = new QTimer();
  connect(leftBlinkerTimer, SIGNAL(timeout()), this, SLOT(leftBlinkerBlinks()));

  this->rightBlinkerTimer = new QTimer();
  connect(rightBlinkerTimer, SIGNAL(timeout()), this,
          SLOT(rightBlinkerBlinks()));

  this->emergentBlinkerTimer = new QTimer();
  connect(emergentBlinkerTimer, SIGNAL(timeout()), this,
          SLOT(emergentBlinkerBlinks()));
}

void toggleBlinker(bool &blinkerStatus) {
  blinkerStatus = (blinkerStatus == BLINKER_ON) ? BLINKER_OFF : BLINKER_ON;
}

void Blinkers::setLeftBlinkerState(bool state) {
  /* Toggle the blinker state because BLINKER_ON/BLINKER_OFF is the same key. */
  if (state == BLINKER_ON && !leftBlinkerTimer->isActive()) {
    leftBlinkerTimer->start(BLINKER_PULSE_DELAY);
  } else {
    leftBlinkerTimer->stop();
    leftBlinkerState = BLINKER_ON;
  }

  emergentBlinkerTimer->stop();
  rightBlinkerState = BLINKER_ON;
  rightBlinkerTimer->stop();
  emit leftBlinkerStateChangedSignal();
  emit rightBlinkerStateChangedSignal();
}

void Blinkers::leftBlinkerBlinks() {
  toggleBlinker(leftBlinkerState);
  emit leftBlinkerStateChangedSignal();
  leftBlinkerTimer->start(BLINKER_PULSE_DELAY);
}

void Blinkers::setRightBlinkerState(bool state) {
  if (state == BLINKER_ON && !rightBlinkerTimer->isActive()) {
    rightBlinkerTimer->start(BLINKER_PULSE_DELAY);
    this->leftBlinkerState = BLINKER_OFF;
    this->emergentBlinkerState = BLINKER_OFF;
  } else {
    rightBlinkerTimer->stop();
    rightBlinkerState = BLINKER_ON;
  }

  emergentBlinkerTimer->stop();
  leftBlinkerTimer->stop();
  leftBlinkerState = BLINKER_ON;
  emit leftBlinkerStateChangedSignal();
  emit rightBlinkerStateChangedSignal();
}

void Blinkers::rightBlinkerBlinks() {
  toggleBlinker(rightBlinkerState);
  rightBlinkerTimer->start(BLINKER_PULSE_DELAY);
  emit rightBlinkerStateChangedSignal();
}

void Blinkers::setEmergentBlinkerState(bool state) {
  /* Toggle the blinker state because BLINKER_ON/BLINKER_OFF is the same key. */
  if (state == BLINKER_ON && !emergentBlinkerTimer->isActive()) {
    emergentBlinkerTimer->start(BLINKER_PULSE_DELAY);
  } else {
    emergentBlinkerTimer->stop();
  }
  emit emergentBlinkerStateChangedSignal();
}

void Blinkers::emergentBlinkerBlinks() {
  toggleBlinker(emergentBlinkerState);
  emit emergentBlinkerStateChangedSignal();
  emergentBlinkerTimer->start(BLINKER_PULSE_DELAY);
}

bool Blinkers::getLeftBlinkerState() { return this->leftBlinkerState; }

bool Blinkers::getRightBlinkerState() { return this->rightBlinkerState; }

bool Blinkers::getEmergentBlinkerState() { return this->emergentBlinkerState; }

#include <Arduino.h>
#include "ConstantCurrent.h"

void ConstantCurrent::init() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, 1);
  pinMode(INC_PIN, OUTPUT);
  digitalWrite(INC_PIN, 1);
  pinMode(UP_PIN, OUTPUT);
  // assume worst case 100 = full power and reduce it to 0 on startup
  potiPos = 100;
  _targetPercent = 0;
  adjustPotiToTarget();
}

void ConstantCurrent::adjustPotiToTarget() {
  auto target = powered? _targetPercent : 0;
  int change = target - potiPos;

  digitalWrite(INC_PIN, 1);
  digitalWrite(CS_PIN, 0);
  delay(1);

  if (change > 0) {
    digitalWrite(UP_PIN, 1);
  } else {
    digitalWrite(UP_PIN, 0);
  }

  int change_abs = abs(change);
  digitalWrite(INC_PIN, 1);
  for (int i = 0; i < change_abs; i++) {
    digitalWrite(INC_PIN, 0);
    delay(1);
    digitalWrite(INC_PIN, 1);
    delay(1);
  }
  potiPos += change;

  digitalWrite(CS_PIN, 1);
}

void ConstantCurrent::setOuputState(bool on) {
  powered = on;
  adjustPotiToTarget();
}

void ConstantCurrent::changeTargetPercent(int delta) {
    setTargetPercent(_targetPercent + delta);
}
void ConstantCurrent::setTargetPercent(int value) {
    _targetPercent = value;
    if (_targetPercent > 100) _targetPercent = 100;
    if (_targetPercent < 0) _targetPercent = 0;
    adjustPotiToTarget();
}
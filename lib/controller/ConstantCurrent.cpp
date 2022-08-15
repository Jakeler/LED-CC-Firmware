#include <Arduino.h>
#include "ConstantCurrent.h"

void ConstantCurrent::init() {
  pinMode(csPin_, OUTPUT);
  digitalWrite(csPin_, 1);
  pinMode(incPin_, OUTPUT);
  digitalWrite(incPin_, 1);
  pinMode(upPin_, OUTPUT);
  // assume worst case 100 = full power and reduce it to 0 on startup
  potiPos_ = 100;
  targetPercent_ = 0;
  adjustPotiToTarget();
}

void ConstantCurrent::adjustPotiToTarget() {
  auto target = powered_? targetPercent_ : 0;
  int change = target - potiPos_;

  digitalWrite(incPin_, 1);
  digitalWrite(csPin_, 0);
  delay(1);

  if (change > 0) {
    digitalWrite(upPin_, 1);
  } else {
    digitalWrite(upPin_, 0);
  }

  int change_abs = abs(change);
  digitalWrite(incPin_, 1);
  for (int i = 0; i < change_abs; i++) {
    digitalWrite(incPin_, 0);
    delay(1);
    digitalWrite(incPin_, 1);
    delay(1);
  }
  potiPos_ += change;

  digitalWrite(csPin_, 1);
}

void ConstantCurrent::setOuputState(bool on) {
  powered_ = on;
  adjustPotiToTarget();
}

void ConstantCurrent::changeTargetPercent(int delta) {
    setTargetPercent(targetPercent_ + delta);
}
void ConstantCurrent::setTargetPercent(int value) {
    targetPercent_ = value;
    if (targetPercent_ > 100) targetPercent_ = 100;
    if (targetPercent_ < 0) targetPercent_ = 0;
    adjustPotiToTarget();
}
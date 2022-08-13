#include <Arduino.h>
#include "ConstantCurrent.h"

void ConstantCurrent::init() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, 1);
  pinMode(INC_PIN, OUTPUT);
  digitalWrite(INC_PIN, 1);
  pinMode(UP_PIN, OUTPUT);
  setPercent(0);
}

void ConstantCurrent::setPercent(int target) {
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
#include <Arduino.h>
#include "ButtonsBeep.h"

bool ButtonInterface::chooseButton() {
    uint16_t value = analogRead(BTN_PIN);

    if (value < 10) handleRIGHT();
    else if (value < 150) handleUP();
    else if (value < 300) handleDOWN();
    else if (value < 500) handleLEFT();
    else if (value < 700) handleSELECT();
    else {
        // no Button
        return false;
    }
    return true;
};

void Beeper::init() {
    digitalWrite(BEEP_PIN, 0);
    pinMode(BEEP_PIN, OUTPUT);
}

void Beeper::beep(int duration, int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BEEP_PIN, 1);
    delay(duration);
    digitalWrite(BEEP_PIN, 0);
    delay(max(duration, 50));
  }
}
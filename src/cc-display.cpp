#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "Sensors.h"
#include "ConstantCurrent.h"

#define BTN_PIN A2
#define BEEP_PIN 10

LiquidCrystal lcd(5, 4, 9, 8, 7, 6);
ConstantCurrent cc;

void readButtons() {
  auto handleUP = []() {
    cc.changeTargetPercent(+2);
    lcd.print("UP");
  };
  auto handleDOWN = []() {
    cc.changeTargetPercent(-2);
    lcd.print("DOWN");
  };
  auto handleRIGHT = []() {
    lcd.print("RIGHT");
  };
  auto handleLEFT = []() {
    lcd.print("LEFT");
  };
  auto handleSELECT = []() {
    cc.setOuputState(!cc.isOutputEnabled);
    lcd.print("SLT");
  };

  uint16_t value = analogRead(BTN_PIN);
  if (value < 10) handleRIGHT();
  else if (value < 150) handleUP();
  else if (value < 300) handleDOWN();
  else if (value < 500) handleLEFT();
  else if (value < 700) handleSELECT();
  else {
    // no Button
  }
}

void updateLcd() {
  auto data = getCurrentPowerData();
  lcd.clear();

  char state = cc.isOutputEnabled? '=' : ' ';
  lcd.print(String(data.outputVoltage, 1) + "/" + String(data.batteryVoltage, 1) + "V ");
  lcd.print(state + String(cc.targetPercent) + "%");

  lcd.setCursor(0, 1);
  lcd.print(String(data.outputAmps, 2) + "A  " + String(data.outputWatts, 1) + "W ");
}

void setup() {
  lcd.begin(16, 2);
  cc.init();
}

void loop() {
  updateLcd();
  readButtons();
  delay(200);
}
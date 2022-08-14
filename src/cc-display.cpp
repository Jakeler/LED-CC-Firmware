#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "Sensors.h"
#include "ConstantCurrent.h"
#include "ButtonsBeep.h"

LiquidCrystal lcd(5, 4, 9, 8, 7, 6);
ConstantCurrent cc;
Beeper beeper;

class Buttons: public ButtonInterface {
  void handleUP() {
    cc.changeTargetPercent(+2);
    lcd.print("UP");
  };
  void handleDOWN() {
    cc.changeTargetPercent(-2);
    lcd.print("DOWN");
  };
  void handleRIGHT() {
    beeper.beep(10, 3);
    lcd.print("RIGHT");
  };
  void handleLEFT() {
    beeper.beep(10, 1);
    lcd.print("LEFT");
  };
  void handleSELECT() {
    cc.setOuputState(!cc.isOutputEnabled);
    lcd.print("SLT");
  };
};
Buttons btns;

void updateLcd() {
  auto data = getCurrentPowerData();
  lcd.clear();

  auto state = cc.isOutputEnabled? '=' : ' ';
  lcd.print(String(data.outputVoltage, 1) + "/" + String(data.batteryVoltage, 1) + "V ");
  lcd.print(state + String(cc.targetPercent) + "%");

  lcd.setCursor(0, 1);
  lcd.print(String(data.outputAmps, 2) + "A  " + String(data.outputWatts, 1) + "W ");
}

void setup() {
  lcd.begin(16, 2);
  cc.init();
  beeper.init();
}

void loop() {
  btns.chooseButton();
  updateLcd();
  delay(200);
}
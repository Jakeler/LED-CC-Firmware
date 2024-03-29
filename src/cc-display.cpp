#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "Sensors.h"
#include "ConstantCurrent.h"
#include "ButtonsBeep.h"

LiquidCrystal lcd(5, 4, 9, 8, 7, 6);
ConstantCurrent cc(INC_PIN, UP_PIN, CS_PIN);
PowerSensor ps(VIN_PIN, VOUT_PIN, AOUT_PIN);
Beeper beeper(BEEP_PIN);

class Buttons: public ButtonInterface {
  using ButtonInterface::ButtonInterface;

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
Buttons btns(BTN_PIN);

void updateLcd() {
  ps.readCurrentData();
  lcd.clear();

  auto state = cc.isOutputEnabled? '=' : ' ';
  lcd.print(String(ps.outputVoltage, 1) + "/" + String(ps.batteryVoltage, 1) + "V ");
  lcd.print(state + String(cc.targetPercent) + "%");

  lcd.setCursor(0, 1);
  lcd.print(String(ps.outputAmps, 2) + "A  " + String(ps.outputWatts, 1) + "W ");
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
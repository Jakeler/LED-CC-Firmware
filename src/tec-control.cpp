#include <Arduino.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Sensors.h"
#include "ConstantCurrent.h"
#include "ButtonsBeep.h"

LiquidCrystal lcd(5, 4, 9, 8, 7, 6);
OneWire oneWire(3);
DallasTemperature sensors(&oneWire);

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
  
  float setpoint = 15.0;
  bool regulating = true;
  float temps[] = {sensors.getTempCByIndex(0), sensors.getTempCByIndex(1), setpoint};
  lcd.clear();
  auto onChar = regulating? '>' : ' ';
  lcd.print(String(temps[0], 1) + "C" + onChar + String(temps[1], 1) + "/" + String(temps[2], 1) + "C");

  lcd.setCursor(0, 1);
  auto stateChar = cc.isOutputEnabled? '=' : ' ';
  lcd.print(String(ps.outputAmps, 2) + "A" + stateChar + String(cc.targetPercent) + "%");
  lcd.print(" " + String(ps.outputWatts, 2) + "W");
}

void setup() {
  lcd.begin(16, 2);
  cc.init();
  beeper.init();

  sensors.begin();
  sensors.setResolution(11);
}

void loop() {
  btns.chooseButton();
  sensors.requestTemperatures(); // takes 375 ms at 11 bit
  updateLcd();
  // delay(200);
}
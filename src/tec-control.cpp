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
  lcd.clear();

  auto state = cc.isOutputEnabled? '=' : ' ';
  lcd.print(String(ps.outputVoltage, 1) + "/" + String(ps.batteryVoltage, 1) + "V ");
  lcd.print(state + String(cc.targetPercent) + "%");

  lcd.setCursor(0, 1);
  lcd.print(String(ps.outputAmps, 2) + "A  " + String(ps.outputWatts, 1) + "W " + String(sensors.getDeviceCount()) + " dv");
}

void setup() {
  lcd.begin(16, 2);
  cc.init();
  beeper.init();

  Serial.begin(9600);
  sensors.begin();
  sensors.setResolution(11);
  Serial.println(sensors.getDeviceCount());
  Serial.println(sensors.getResolution());
}

void loop() {
  btns.chooseButton();
  updateLcd();
  delay(200);

  sensors.requestTemperatures();
  Serial.println("Sens 1 " + String(sensors.getTempCByIndex(0)));
  Serial.println("Sens 2 " + String(sensors.getTempCByIndex(1)));
  
}
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <QuickPID.h>
#include "Sensors.h"
#include "ConstantCurrent.h"
#include "ButtonsBeep.h"

#define DEBUG_PID

LiquidCrystal lcd(5, 4, 9, 8, 7, 6);
OneWire oneWire(3);
DallasTemperature sensors(&oneWire);

ConstantCurrent cc(INC_PIN, UP_PIN, CS_PIN);
PowerSensor ps(VIN_PIN, VOUT_PIN, AOUT_PIN);
Beeper beeper(BEEP_PIN);

float setpoint = 15.0, input, output;
float Kp = 10, Ki = 4, Kd = 5;
QuickPID myPID(&input, &output, &setpoint, Kp, Ki, Kd, QuickPID::Action::reverse);

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
    setpoint += 2;
    lcd.print("RIGHT");
  };
  void handleLEFT() {
    setpoint -= 2;
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
  
  bool regulating = true;
  float temps[] = {sensors.getTempCByIndex(0), sensors.getTempCByIndex(1), setpoint};
  input = temps[1];
  lcd.clear();
  auto onChar = regulating? '>' : ' ';
  lcd.print(String(temps[0], 1) + "\xDF" + onChar + String(temps[1], 1) + "/" + String(temps[2], 1) + "\xDF");

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

  myPID.SetOutputLimits(0, 64);
  myPID.SetMode(QuickPID::Control::automatic);

  #ifdef DEBUG_PID
    Serial.begin(9600);
  #endif
}

void loop() {
  btns.chooseButton();
  sensors.requestTemperatures(); // takes 375 ms at 11 bit
  // delay(200);
  updateLcd();
  myPID.Compute();
  cc.setTargetPercent(output);

  #ifdef DEBUG_PID
    Serial.println("in: " + String(input) + "/" + String(setpoint));
    Serial.println("out: " + String(output) + " = " + String(sqrt(output*64)));
    Serial.println("P: " + String(myPID.GetPterm()));
    Serial.println("I: " + String(myPID.GetIterm()));
    Serial.println("D: " + String(myPID.GetDterm()));
  #endif
}
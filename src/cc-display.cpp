#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define VIN_PIN A1
#define VOUT_PIN A3
#define AOUT_PIN A0
#define AOUT_MAX 12.8205
#define VIN_MAX 20.957

#define INC_PIN 12
#define UP_PIN 11
#define CS_PIN 13

#define BTN_PIN A2
#define BEEP_PIN 10


int potiPos = 100, targetPower = 0;
bool powered = false;

LiquidCrystal lcd(5, 4, 9, 8, 7, 6);

void setPoti(int target) {
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


void initPoti() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, 1);
  pinMode(INC_PIN, OUTPUT);
  digitalWrite(INC_PIN, 1);
  pinMode(UP_PIN, OUTPUT);
  setPoti(0);
}

void readButtons() {
  auto handleUP = []() {
    targetPower += 2;
    if (targetPower > 100) targetPower = 100;
    setPoti(powered? targetPower : 0);
    lcd.print("UP");
  };
  auto handleDOWN = []() {
    targetPower -= 2;
    if (targetPower < 0) targetPower = 0;
    setPoti(powered? targetPower : 0);
    lcd.print("DOWN");
  };
  auto handleRIGHT = []() {
    lcd.print("RIGHT");
  };
  auto handleLEFT = []() {
    lcd.print("LEFT");
  };
  auto handleSELECT = []() {
    powered = !powered;
    setPoti(powered? targetPower : 0);
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
  lcd.clear();
  float batV = analogRead(VIN_PIN)/1023.0*VIN_MAX;
  float outV = analogRead(VOUT_PIN)/1023.0*VIN_MAX;
  float outA = analogRead(AOUT_PIN)/1023.0*AOUT_MAX;
  char state = powered? '=' : ' ';
  lcd.print(String(outV, 1) + "/" + String(batV, 1) + "V ");
  lcd.print(state + String(targetPower) + "W");

  lcd.setCursor(0, 1);
  lcd.print(outA);
  lcd.print("A  ");
}

void setup() {
  lcd.begin(16, 2);
  initPoti();
}

void loop() {
  updateLcd();
  readButtons();
  delay(200);
}
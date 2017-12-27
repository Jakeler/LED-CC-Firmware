#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define VIN A1
#define VOUT A3
#define AOUT A0
#define AOUT_MAX 12.8205
#define BTN A2
#define VIN_MAX 20.957
#define LI_cell 3.0
#define LI_minV 3.5
#define INC_PIN 12
#define UP_PIN 11
#define CS_PIN 13
#define BEEP_PIN 10
#define MAX_STANDBY 60

int pot_pos = 100, power = 50, standby_count = 0;

LiquidCrystal lcd(5, 4, 9, 8, 7, 6);

uint8_t light[8]  = {B0,B00011,B00111,B01111,B11111,B01111,B00111,B00011};
uint8_t shine[8] = {0x0,0x0,0x1F,0x0,0x1F,0x0,0x1F,0x0};

uint8_t start[8]  = {B1,B1,B1,B1,B1,B1,B1,B1};
uint8_t end[8]  = {B10000,B10000,B10000,B11000,B11000,B10000,B10000,B10000};
uint8_t leer_mid[8]  = {B11111,B0,B0,B0,B0,B0,B0,B11111};
uint8_t full_mid[8] = {0x1F,0x0,0x1F,0x1F,0x1F,0x1F,0x0,0x1F};

int buttons[5] = {10, 150, 300, 500, 700};

void beep(int, int);

void initLCD() {
  lcd.begin(16,2);
  lcd.createChar(3, start);
  lcd.createChar(0, leer_mid);
  lcd.createChar(1, full_mid);
  lcd.createChar(4, end);
  lcd.createChar(5, light);
  lcd.createChar(6, shine);

  lcd.home();

  lcd.print("-- Self Test --");
  lcd.setCursor(0, 1);
  lcd.print("TODO");

  delay(1000);
}


void setPot(int change) {
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
    digitalWrite(INC_PIN, 1);
    delay(1);
    digitalWrite(INC_PIN, 0);
    delay(1);
  }
  pot_pos += change;
  digitalWrite(CS_PIN, 0);
}

void initPot() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, 1);
  pinMode(INC_PIN, OUTPUT);
  digitalWrite(INC_PIN, 1);
  pinMode(UP_PIN, OUTPUT);
  setPot(-100);
}

void readButtons() {
  uint16_t value = analogRead(BTN);
  if (value < 10) {
    // RIGHT
    power += 10;
    beep(50,1);

  } else if (value < 150) {
    // UP
    power++;
    beep(20,1);

  } else if (value < 300) {
    // DOWN
    power--;
    beep(20,1);

  } else if (value < 500) {
    // LEFT
    power -= 10;
    beep(50,1);

  } else if (value < 700) {
    // SELECT
    if (power == 0) {
      power = 100;
      beep(100,1);
    } else {
      power = 0;
      beep(100,1);
    }
  } else {
    // no Button
  }
  if (power > 100) {
    power = 100;
    beep(100,1);
  } else if (power < 0) {
    power = 0;
    beep(100,1);
  }
}

void checkStandby() {
  if (power == 0) {
    standby_count++;
  } else {
    standby_count = 0;
  }
  if (standby_count >= MAX_STANDBY) {
    beep(100, 3);
    standby_count -= 10;
  }
}

void beep(int duration, int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BEEP_PIN, 1);
    delay(duration);
    digitalWrite(BEEP_PIN, 0);
    delay(duration);
  }
}

void setup() {

  digitalWrite(BEEP_PIN, 0);
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(VIN, INPUT);
  pinMode(BTN, INPUT);

  initPot();
  initLCD();
  beep(200, 2);
}

void updateLcd(float batV, float charge) {
  lcd.clear();
  lcd.print(String(batV, 1) + "V");
  lcd.setCursor(5, 0);
  lcd.write(3);
  for (float i = 0.125; i < 1.1; i+= 0.125) {
    if(charge >= i) {
      lcd.write(1);
    } else {
      lcd.write(byte(0));
    }
  }
  lcd.write(4);

  lcd.setCursor(0, 1);
  lcd.write(5);
  for (float i = 0.0; i < 1.1; i+= 0.25) {
    if(pot_pos/100.0 >= i) {
      lcd.write(6);
    } else {
      lcd.print(" ");
    }
  }
  lcd.print(pot_pos);
  //lcd.print(" %  S:");
  //lcd.print(standby_count/2);
}

void loop() {
    readButtons();
    float batV = analogRead(VIN)/1023.0*VIN_MAX;
    float outV = analogRead(VOUT)/1023.0*VIN_MAX;
    float outA = analogRead(AOUT)/1023.0*AOUT_MAX;
    float cellV = batV/LI_cell;
    float charge = (cellV-LI_minV)/(4.2-LI_minV);
    if (charge <= 0.0) {
      power = 0;
      setPot(power-pot_pos);
      beep(500, 3);
    }
    setPot(power-pot_pos);

    checkStandby();

    updateLcd(batV, charge);
    lcd.print("% ");
    lcd.print(outA*outV);

    //lcd.print(String(pot*100, 0) + "% " + String(pot*10, 1) + "W");
    delay(500);
}

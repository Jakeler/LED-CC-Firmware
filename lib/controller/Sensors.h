#define VIN_PIN A1
#define VOUT_PIN A3
#define AOUT_PIN A0
#define AOUT_MAX 12.8205
#define VIN_MAX 20.957

struct powerData {
  float batteryVoltage;
  float outputVoltage;
  float outputAmps;
  float outputWatts;
};

powerData getCurrentPowerData();
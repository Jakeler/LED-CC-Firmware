#define VIN_PIN A1
#define VOUT_PIN A3
#define AOUT_PIN A0
#define AOUT_MAX 12.8205
#define VIN_MAX 20.957


class PowerSensor {

  private:
    const uint8_t inVoltPin_, outVoltPin_, outAmpPin_;
    const double maxVolt_, maxAmpOut_;

  public:
    PowerSensor(uint8_t inVoltPin, uint8_t outVoltPin, uint8_t outAmpPin, 
      double maxVolt = VIN_MAX, double maxAmpOut = AOUT_MAX) :
      inVoltPin_{inVoltPin}, outVoltPin_{outVoltPin}, outAmpPin_{outAmpPin}, 
      maxVolt_{maxVolt}, maxAmpOut_{maxAmpOut} {};

    float batteryVoltage;
    float outputVoltage;
    float outputAmps;
    float outputWatts;

    void readCurrentData();
};

#include <Arduino.h>
#include "Sensors.h"

powerData getCurrentPowerData() {
    powerData d;
    d.batteryVoltage = analogRead(VIN_PIN)/1023.0*VIN_MAX;
    d.outputVoltage = analogRead(VOUT_PIN)/1023.0*VIN_MAX;
    d.outputAmps = analogRead(AOUT_PIN)/1023.0*AOUT_MAX;
    d.outputWatts = d.outputVoltage * d.outputAmps;
    return d;
}
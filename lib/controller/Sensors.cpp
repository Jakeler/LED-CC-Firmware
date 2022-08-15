#include <Arduino.h>
#include "Sensors.h"

void PowerSensor::readCurrentData() {
    batteryVoltage = analogRead(inVoltPin_)/1023.0*maxVolt_;
    outputVoltage = analogRead(outVoltPin_)/1023.0*maxVolt_;
    outputAmps = analogRead(outAmpPin_)/1023.0*maxAmpOut_;
    outputWatts = outputVoltage * outputAmps;
}
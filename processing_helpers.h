#ifndef PROCESSING_HELPERS_H
#define PROCESSING_HELPERS_H

#include <Arduino.h>

float getVoltage(int count);
int getMoisturePercentage(int count);
int getRelativeMoisture(int soil_moisture, int atm_moisture);

#endif
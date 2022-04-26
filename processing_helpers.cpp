#include "processing_helpers.h"

float getVoltage(int count) {
  float voltage = (float(count)/1023.0)*3.3;
  return(voltage);
}

int getMoisturePercentage(int count) {
  int moisture_value = map(count, 445, 890, 0, 100); // wet: 445, dry: 890
  int moisture_percentage = 100 - moisture_value; 
  
  return(moisture_percentage);
}

int getRelativeMoisture(int soil_moisture, int atm_moisture) {
  int rel_moisture = soil_moisture - atm_moisture;
  return(rel_moisture);
}
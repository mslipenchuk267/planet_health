#ifndef DISPLAY_HELPERS_H
#define DISPLAY_HELPERS_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void displayText(Adafruit_SSD1306 display, const char* msg);
void displayMoisture(Adafruit_SSD1306 display, int rel_moisture_value, int soil_moisture_value, int atm_moisture_value);

#endif
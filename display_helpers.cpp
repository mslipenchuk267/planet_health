#include "display_helpers.h"

void displayText(Adafruit_SSD1306 display, const char* msg) {
  display.clearDisplay();                // Clear display buffer
  display.setTextSize(2);                     // 2:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(4,0);             // Start at top-left corne
  display.println(msg);
  display.display();
}

void displayMoisture(Adafruit_SSD1306 display, int rel_moisture_value, int soil_moisture_value, int atm_moisture_value) {
  display.clearDisplay();               // Clear display buffer
  display.setTextSize(2);                    // 4:1 pixel scale
  display.setTextColor(SSD1306_WHITE);       // Draw white text
  display.setCursor(0,0);           // Start at top-left corner
  display.print("Rel : ");
  display.print(rel_moisture_value);
  display.println(F(" %"));
  display.setCursor(0,20); 
  display.print("Soil: ");
  display.print(soil_moisture_value);
  display.println(F(" %"));
  display.setCursor(0,40);
  display.print("Atm : ");
  if (atm_moisture_value < 10) {
    display.print(" ");
  }
  display.print(atm_moisture_value);
  display.println(F(" %"));
  display.display();
}
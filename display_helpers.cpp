#include "display_helpers.h"

void displayText(Adafruit_SSD1306 display, const char* msg) {
  display.clearDisplay();                // Clear display buffer
  display.setTextSize(2);                     // 2:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(4,0);             // Start at top-left corne
  display.println(msg);
  display.display();
}

void displayMoisture(Adafruit_SSD1306 display, int moisture_value) {
  display.clearDisplay();               // Clear display buffer
  display.setTextSize(4);                    // 4:1 pixel scale
  display.setTextColor(SSD1306_WHITE);       // Draw white text
  display.setCursor(0,0);           // Start at top-left corner
  display.print(moisture_value);
  display.println(F(" %"));
  display.display();
}
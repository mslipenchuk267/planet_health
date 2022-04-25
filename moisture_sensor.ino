/* OLED Display Pin Information
Arduino:
	CS	10
	DC	7
	RST	8
SPI:	
	DIN	11	
	CLK	13
IIC:
	DIN	SDA
	CLK	SCL
*/
#include <SPI.h>
#include <Wire.h>
#include <WiFi101.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "secrets.h" // ignore this in git repo, or make it a github secret

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

int MOISTURE_SENSOR_PIN = A0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Diplay Error: SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  displayText("Wifi...");

  // Connect to Wifi
  connectWifi(status, ssid, pass);

  display.clearDisplay();
}

void loop() {
  // Read analog input of moisture sensor
  int raw_value = analogRead(MOISTURE_SENSOR_PIN); // random(1024); // 

  // Map analog raw value to 0-100 range moisture %
  int moisture_value = map(raw_value, 0, 1023, 0, 100);
  moisture_value = 100 - moisture_value;

  // (TODO) Adjust sensor for atmospheric mositure

  Serial.print("Soil Moisture: ");
  Serial.print(moisture_value);
  Serial.println(" %");

  displayMoisture(moisture_value);
  delay(1000); // Display for 1 second before next iteration
}

void displayText(const char* msg) {
  display.clearDisplay();   // Clear display buffer
  display.setTextSize(2);             // 2:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(4,0);             // Start at top-left corne
  display.println(msg);
  display.display();
}

void displayMoisture(int moisture_value) {
  display.clearDisplay();               // Clear display buffer
  display.setTextSize(4);             // 4:1 pixel scale
  display.setTextColor(SSD1306_WHITE);       // Draw white text
  display.setCursor(0,0);           // Start at top-left corner
  display.print(moisture_value);
  display.println(F(" %"));
  display.display();
}

// Turn on LED for 1 second
void blinkLed() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                       // wait for a second
}

void connectWifi(int status, char ssid[], char pass[]) {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");

  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");
}

void printData() {
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

}
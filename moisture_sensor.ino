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
#include <ArduinoMqttClient.h>

#include "display_helpers.h"
#include "secrets.h" 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
char sensor_name[] = SECRET_SENSOR_NAME;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

int SOIL_MOISTURE_SENSOR_PIN = A0;
int ATM_MOISTURE_SENSOR_PIN = A1;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = SECRET_IP;
int        port     = 1883;
const char topic[]  = "moisture";

//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;


void setup() {
  Serial.begin(9600);
  while (!Serial);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Diplay Error: SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  displayText(display, "Wifi...");

  // Connect to Wifi
  connectWifi(status, ssid, pass);

  displayText(display, "MQTT...");


  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("Connected to MQTT broker");
  Serial.println();

  display.clearDisplay();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  // Read analog input of moisture sensor
  int soil_raw_count = analogRead(SOIL_MOISTURE_SENSOR_PIN); // random(1024); // 
  int atm_raw_count = analogRead(ATM_MOISTURE_SENSOR_PIN); // random(1024); // 

  // Get Voltage of sensors
  float soil_v = (float(soil_raw_count)/1023.0)*3.3;
  float atm_v = (float(atm_raw_count)/1023.0)*3.3;

  // Map analog raw value to 0-100 range moisture %
  int soil_moisture_value = map(soil_raw_count, 445, 890, 0, 100); // wet: 445, dry: 890
  soil_moisture_value = 100 - soil_moisture_value; 

  int atm_moisture_value = map(atm_raw_count, 445, 890, 0, 100); // wet: 445, dry: 890
  atm_moisture_value = 100 - atm_moisture_value;

  int relative_moisture_value = soil_moisture_value - atm_moisture_value; 

  // (TODO) Adjust sensor for atmospheric mositure

  // Send Moisture Value to MQTT broker
  mqttClient.beginMessage(topic);
  mqttClient.print(sensor_name);
  mqttClient.print(",");
  mqttClient.print(soil_v);
  mqttClient.print(",");
  mqttClient.print(atm_v);
  mqttClient.print(",");
  mqttClient.print(soil_raw_count);
  mqttClient.print(",");
  mqttClient.print(atm_raw_count);
  mqttClient.print(",");
  mqttClient.print(soil_moisture_value);
  mqttClient.print(",");
  mqttClient.print(atm_moisture_value);
  mqttClient.print(",");
  mqttClient.print(relative_moisture_value);
  mqttClient.endMessage();

  // Display Moisture Value
  Serial.print("Soil V: ");
  Serial.print(soil_v);
  Serial.print(" | Atm V: ");
  Serial.println(atm_v);

  Serial.print("Soil Count: ");
  Serial.print(soil_raw_count);
  Serial.print(" | Atm Count: ");
  Serial.println(atm_raw_count);
  // Serial
  Serial.print("Soil M.: ");
  Serial.print(soil_moisture_value);
  Serial.print(" % | Atm M.: ");
  Serial.print(atm_moisture_value);
  Serial.print(" % | Rel M.: ");
  Serial.print(relative_moisture_value);
  Serial.println(" %");
  Serial.println("----------------------------------------");
  // OLED
  displayMoisture(display, relative_moisture_value);
  delay(1000); // Display for 1 second before next iteration
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

    // wait 1 seconds for connection:
    delay(1000);
  }
  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");

  Serial.println("----------------------------------------");
  printWifiData();
  Serial.println("----------------------------------------");
}

void printWifiData() {
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
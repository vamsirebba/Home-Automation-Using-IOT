tint switch1_value, switch2_value, switch3_value, switch4_value;
#include<Wire.h>
#define LIGHT 12
#define FAN 13
#define flame_pin 16

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "Ubidots.h"
const char* UBIDOTS_TOKEN = "BBFF-oQGMI6Bxb6J977nb34M51mgCfxe3ht";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "CRK HOME";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "CRKSUMALATHA";      // Put here your Wi-Fi password
const char* VARIABLE_LABEL1 = "light";       // Replace with your variable label
const char* VARIABLE_LABEL2 = "fan";       // Replace with your variable label
const char* DEVICE_LABEL = "5ccf7fbe7bb2";  // Replace with your device label
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

#include <DHT.h>
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t = 0, h = 0;
int flame_value = 0;

void setup()
{
  pinMode(LIGHT, OUTPUT);
  pinMode(FAN, OUTPUT);
  digitalWrite(LIGHT, LOW);
  digitalWrite(FAN, LOW);

  lcd.begin();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    Smart home     ");
  lcd.setCursor(0, 1);
  lcd.print("automation IOT!");
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  dht.begin();
  delay(2000);
}

void loop()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  flame_value = digitalRead(flame_pin);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.print(" *C");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print(h);
  lcd.print(" %");
  delay(500);
  lcd.clear();
  if (flame_value == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("Flame: Danger!");
    lcd.setCursor(0, 1);
    lcd.print("Fire Detected");
    delay(500);
    flame_value = 1;
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Flame: Safe!");
    lcd.setCursor(0, 1);
    lcd.print("Fire not detected");
    delay(500);
    flame_value = 0;
  }
  ubidots.add("Temperature", t);  // Change for your variable name
  ubidots.add("Humidity", h);  // Change for your variable name
  ubidots.add("Flame", flame_value);  // Change for your variable name
  ubidots.send();  // Will send data to a device label that matches the device Id
  // delay(1000);
  switch1_value = ubidots.get(DEVICE_LABEL, VARIABLE_LABEL1);
  Serial.print("Switch1: ");
  Serial.println(switch1_value);

  if (switch1_value == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LIGHT: OFF");
    Serial.println("LIGHT : OFF ");
    digitalWrite(LIGHT, LOW);
    delay(500);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LIGHT: ON");
    Serial.println("LIGHT : ON ");
    digitalWrite(LIGHT, HIGH);
    delay(500);
  }
  switch2_value = ubidots.get(DEVICE_LABEL, VARIABLE_LABEL2);
  Serial.print("Switch2: ");
  Serial.println(switch2_value);
  // delay(500);
  if (switch2_value == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FAN: OFF");
    Serial.println("FAN : OFF ");
    digitalWrite(FAN, LOW);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FAN: ON");
    Serial.println("FAN : ON ");
    digitalWrite(FAN, HIGH);
  }
}


#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <RTClib.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4       // Pin connected to DHT22 data pin
#define DHTTYPE DHT11  // DHT 22 (AM2302)
#define MQ6_PIN 34     // Analog pin for MQ6
#define MQ7_PIN 35     // Analog pin for MQ7
#define BUZZER_PIN 13  // Pin for buzzer
#define ZH03B_RX 17    // ESP32 RX pin connected to ZH03B TX
#define ZH03B_TX 16    // ESP32 TX pin connected to ZH03B RX
#define ZH03B_pwm 26
#define BUTTON_PIN 25

bool displayState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;     

LiquidCrystal_I2C lcd(0x27, 16, 2);

const unsigned long resetInterval = 45000;

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;

struct SensorData {
  int temperature;      // Store as integer to save memory
  int humidity;         // Store as integer to save memory
  int propane_butane;   // Store as integer to save memory
  int carbon_monoxide;  // Store as integer to save memory
  int pm1;
  int pm2_5;
  int pm10;
  uint32_t timestamp;  // Store timestamp as integer (seconds since epoch)
};

SensorData dataLog[30];  // Reduced array size to 30 samples for 10 minutes of data at 20s intervals
int currentIndex = 0;
const float CO_CORRECTION_FACTOR = 0.3;  // Adjust as necessary

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, ZH03B_RX, ZH03B_TX);
  Wire.begin();
  dht.begin();

  //the button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(MQ6_PIN, INPUT);
  pinMode(MQ7_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
}

void loop() {
  if (millis() >= resetInterval) {
    // Reset the variable and timer every 60 seconds
    ESP.restart();
  }
  int currentButtonState = digitalRead(BUTTON_PIN);
  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState == LOW && lastButtonState == HIGH) {
      displayState = !displayState;
    }
  }
  lastButtonState = currentButtonState;
  readAndStoreSensorData();
  checkThresholds();
  delay(3000);

}

void readAndStoreSensorData() {

  int temperature = (int)dht.readTemperature();
  int humidity = (int)dht.readHumidity();
  if (-20 < temperature || 80 > temperature || 5 < humidity || 100 > humidity) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    int temp = (int)temperature;
    int hum = (int)humidity;
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C, ");
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, ");
  }

  int propane_butane = analogRead(MQ6_PIN) * 0.3;   // Replace with calibration for ppm if needed
  int carbon_monoxide = analogRead(MQ7_PIN) * 0.3;  // Apply correction factor
  int pm1 = 0, pm2_5 = 0, pm10 = 0;

  // Read data from ZH03B
  if (Serial2.available() > 0) {
    uint8_t buffer[32];
    int index = 0;

    while (Serial2.available() > 0 && index < 32) {
      buffer[index++] = Serial2.read();
    }

    if (index >= 24 && buffer[0] == 0x42 && buffer[1] == 0x4d) {
      pm1 = (buffer[10] << 8) + buffer[11];
      pm2_5 = (buffer[12] << 8) + buffer[13];
      pm10 = (buffer[14] << 8) + buffer[15];
    }
  }

  if (displayState==HIGH){
    lcd.clear();
    lcd.print("PM2.5: ");
    lcd.print(pm2_5);
    lcd.print(" ug/m3");
    lcd.setCursor(0,1);
    lcd.print("PM1:");
    lcd.print(pm1);
    lcd.print(" PM10:");
    lcd.print(pm10);
    displayState = !displayState;
  } else {
    lcd.clear();
    lcd.print("LPG: ");
    lcd.print(propane_butane);
    lcd.print(" ppm");
    lcd.setCursor(0,1);
    lcd.print("CO: ");
    lcd.print(carbon_monoxide);
    lcd.print(" ppm");
    displayState = !displayState;
  }



  Serial.print("Propane/Iso-butane: ");
  Serial.print(propane_butane);
  Serial.print(" ppm, Carbon Monoxide: ");
  Serial.print(carbon_monoxide);
  Serial.print(" ppm, PM1: ");
  Serial.print(pm1);
  Serial.print("µg/m³, PM2.5: ");
  Serial.print(pm2_5);
  Serial.print(" µg/m³, PM10: ");
  Serial.println(pm10);
  Serial.print(" µg/m³\n");
}


void checkThresholds() {
  int propane_butane = analogRead(MQ6_PIN)*0.3;
  int carbon_monoxide = analogRead(MQ7_PIN) * 0.3;

  if (propane_butane*0.3 > 800 || carbon_monoxide*0.3 > 150) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(650);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <RTClib.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define DHTPIN 4          // Pin connected to DHT22 data pin
#define DHTTYPE DHT22     // DHT 22 (AM2302)
#define MQ6_PIN 34        // Analog pin for MQ6
#define MQ7_PIN 35        // Analog pin for MQ7
#define BUZZER_PIN 13     // Pin for buzzer
#define ZH03B_RX 17       // ESP32 RX pin connected to ZH03B TX
#define ZH03B_TX 16       // ESP32 TX pin connected to ZH03B RX

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;

const char* ssid = "ORCP | FT-10";
const char* password = "orcp1234";

struct SensorData {
    int temperature;           // Store as integer to save memory
    int humidity;              // Store as integer to save memory
    int propane_butane;        // Store as integer to save memory
    int carbon_monoxide;       // Store as integer to save memory
    int pm1;
    int pm2_5;
    int pm10;
    uint32_t timestamp;        // Store timestamp as integer (seconds since epoch)
};

SensorData dataLog[30];       // Reduced array size to 30 samples for 10 minutes of data at 20s intervals
int currentIndex = 0;

WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, ZH03B_RX, ZH03B_TX); // Initialize ZH03B serial communication
    Wire.begin();
    
    dht.begin();
    
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
    }
    if (!rtc.isrunning()) {
        rtc.adjust(DateTime(2024, 11, 10, 18, 53, 0));
    }
    
    pinMode(MQ6_PIN, INPUT);
    pinMode(MQ7_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    server.begin();
}

void loop() {
    readAndStoreSensorData();
    checkThresholds();
    handleClientRequest();

    delay(20000); // Collect data every 20 seconds
}

void readAndStoreSensorData() {
    DateTime now = rtc.now();

    // Read temperature and humidity from DHT22 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check if the readings are valid
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
    } else {
        // Store valid data
        int temp = (int)temperature;
        int hum = (int)humidity;

        // Read data from MQ6 and MQ7
        int propane_butane = analogRead(MQ6_PIN); // Replace with calibration for ppm if needed
        int carbon_monoxide = analogRead(MQ7_PIN); // Replace with calibration for ppm if needed
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

        // Save the current reading to the log array
        dataLog[currentIndex] = { temp, hum, propane_butane, carbon_monoxide, pm1, pm2_5, pm10, now.unixtime() };
        currentIndex = (currentIndex + 1) % 30; // Keep index within bounds for 10 minutes of data

        // Print the valid readings to the serial monitor
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.print(" °C, ");
        Serial.print("Humidity: ");
        Serial.print(hum);
        Serial.print(" %, ");
        Serial.print("Propane/Iso-butane: ");
        Serial.print(propane_butane);
        Serial.print(" ppm, ");
        Serial.print("Carbon Monoxide: ");
        Serial.print(carbon_monoxide);
        Serial.print(" ppm, ");
        Serial.print("PM1: ");
        Serial.print(pm1);
        Serial.print(" µg/m³, ");
        Serial.print("PM2.5: ");
        Serial.print(pm2_5);
        Serial.print(" µg/m³, ");
        Serial.print("PM10: ");
        Serial.print(pm10); // Make sure you print PM10 here
        Serial.println(" µg/m³"); // Add unit for PM10
    }
}

void checkThresholds() {
    int propane_butane = analogRead(MQ6_PIN);
    int carbon_monoxide = analogRead(MQ7_PIN);

    if (propane_butane > 300 || carbon_monoxide > 50) { // Example thresholds
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
        sendAlertToApp();
    }
}

void sendAlertToApp() {
    WiFiClient client = server.available();
    if (client) {
        StaticJsonDocument<200> alertDoc;
        alertDoc["alert"] = "Dangerous levels detected!";
        String output;
        serializeJson(alertDoc, output);
        client.print(output);
        client.stop();
    }
}

void handleClientRequest() {
    WiFiClient client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        client.flush();

        if (request.indexOf("/get_graph_data") != -1) {
            DynamicJsonDocument jsonDoc(3000);
            JsonArray readings = jsonDoc.createNestedArray("readings");
            
            for (int i = 0; i < 30; i++) {
                JsonObject reading = readings.createNestedObject();
                reading["temperature"] = dataLog[i].temperature;
                reading["humidity"] = dataLog[i].humidity;
                reading["propane_butane"] = dataLog[i].propane_butane;
                reading["carbon_monoxide"] = dataLog[i].carbon_monoxide;
                reading["pm1"] = dataLog[i].pm1;
                reading["pm2_5"] = dataLog[i].pm2_5;
                reading["pm10"] = dataLog[i].pm10;
                reading["timestamp"] = dataLog[i].timestamp;
            }

            String output;
            serializeJson(jsonDoc, output);
            client.print(output);
        }
        client.stop();
    }
}

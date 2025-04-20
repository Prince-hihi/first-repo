#include <SoftwareSerial.h>

// Define ZH03B pins for SoftwareSerial
#define ZH03B_RX D7 // Wemos D1 GPIO pin connected to TX of ZH03B
#define ZH03B_TX D6 // Wemos D1 GPIO pin connected to RX of ZH03B

SoftwareSerial zh03bSerial(ZH03B_RX, ZH03B_TX); // Initialize SoftwareSerial

// Variables to store PM values
uint16_t pm1_0, pm2_5, pm10;

void setup() {
  Serial.begin(115200);          // Serial monitor
  zh03bSerial.begin(9600);       // ZH03B baud rate (refer datasheet)
  
  delay(1000);                   // Wait for sensor initialization
  Serial.println("ZH03B Sensor Initialized");
}

void loop() {
  if (zh03bSerial.available() >= 32) { // Data packet is 32 bytes
    uint8_t buffer[32];
    zh03bSerial.readBytes(buffer, 32);
    
    if (validateChecksum(buffer, 32)) { // Check if data is valid
      pm1_0 = (buffer[4] << 8) | buffer[5];
      pm2_5 = (buffer[6] << 8) | buffer[7];
      pm10  = (buffer[8] << 8) | buffer[9];
      
      Serial.print("PM1.0: ");
      Serial.print(pm1_0);
      Serial.print(" µg/m³, PM2.5: ");
      Serial.print(pm2_5);
      Serial.print(" µg/m³, PM10: ");
      Serial.print(pm10);
      Serial.println(" µg/m³\n");
    } else {
      Serial.println("Checksum error: Invalid data\n");
    }
  }
  delay(1000); // Adjust sampling rate as needed
}

// Validate checksum
bool validateChecksum(uint8_t* buffer, uint8_t length) {
  uint16_t checksum = 0;
  for (int i = 0; i < length - 2; i++) {
    checksum += buffer[i];
  }
  return ((checksum & 0xFF) == buffer[length - 2]);
}

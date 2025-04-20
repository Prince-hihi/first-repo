#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Define SPI pins for ESP8266
#define TFT_CS    D8  // GPIO15
#define TFT_DC    D4  // GPIO2
#define TFT_RST   D3  // GPIO0

// Initialize ST7789 TFT display
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  
  // Initialize display
  tft.init(240, 320);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  
  // Display test message
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.print("Hello, ESP8266!");
}

void loop() {
  // Nothing to do in the loop
}

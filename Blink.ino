#include <LiquidCrystal_I2C.h>
#include <wire.h>

#define BUTTON_PIN 25 

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pullup resistor
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    lcd.clear();
    lcd.print("Button Pressed!");
  } else {
    lcd.clear();
    lcd.print("Button Released!");
  }
  delay(100); // Small delay for debounce
}

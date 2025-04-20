#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_PIN 25 // GPIO pin for the button

// Initialize the LCD (update I2C address if necessary)
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool lastButtonState = LOW; // Previous button state
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 50ms debounce delay

bool displayState = 0; // State variable: 0 or 1


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor for button
  lcd.init();                        // Initialize the LCD
  lcd.backlight();                   // Turn on the backlight
}

void loop() {
  // Read the current button state
  bool reading = digitalRead(BUTTON_PIN);

  // Check if the button state has changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  // If the button is stable and pressed, toggle the display state
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == HIGH && lastButtonState == LOW) {
      // Toggle the state
      displayState = !displayState; // Switch between 0 and 1
      updateLCD(); // Update the LCD display
    }
  }

  reading = lastButtonState ; // Save the current button state
}

// Function to update the LCD display based on the state
void updateLCD() {
  lcd.clear(); // Clear the display
  if (displayState == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Display State 0");
    lcd.setCursor(0, 1);
    lcd.print("Hello, World!");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Display State 1");
    lcd.setCursor(0, 1);
    lcd.print("ESP32 Rocks!");
  }
}

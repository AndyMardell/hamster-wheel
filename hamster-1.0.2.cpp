/////////////////////////
// PIN MAPPING
/////////////////////////
// TX = 1
// RX = 3
// D1 = 5
// D2 = 4
// D3 = 0
// D4 = 2
// D0 = 16
// D5 = 14
// D6 = 12
// D7 = 13
// D8 = 15
/////////////////////////

// Imports
#include <LiquidCrystal_I2C.h>

// Setup LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
volatile byte revolutions;
unsigned int rpm;
unsigned long timeold;
const int hallPin = 5; // Hall Effect Sensor on Pin D1
const int buttonPin = 4;
const int screen = 1; // Screen 1 or 2

// Debugging
const int debugging = 1;
const int logging = 1;

// Setup
void setup() {

  // Begin Serial
  Serial.begin(115200);

  // Attach interupt to hallPin, call magnetDetect()
  attachInterrupt(hallPin, magnetDetect, RISING);

  // Attach interupt to button, call changeDisplay()
  attachInterrupt(buttonPin, changeDisplay, RISING);

  // Set up display one
  setupDisplayOne();

  // Reset vars
  revolutions = 0;
  rpm = 0;
  timeold = 0;
  
}

// Main Program
void loop() {

  if (revolutions >= 20) { 
    
    // Calculate RPM
    rpm = 30*1000/(millis() - timeold)*revolutions;
    timeold = millis();
    revolutions = 0;
    
    if (debugging) {
      Serial.println(rpm,DEC); 
    }
    
  }

  // Update Display
  if (screen == 1) {
    updateDisplayOne();  
  } else {
    updateDisplayTwo();
  }
  
  
}

//This function is called whenever a magnet/interrupt is detected by the arduino
void magnetDetect() {

  if (logging) {
    Serial.println("Detected");
  }

  revolutions++;
  
}

// Change Display
void changeDisplay() {

  if (screen == 1) {
    setupDisplayTwo();
    screen = 2;
  } else {
    setupDisplayOne();
    screen = 1;
  }

}

// Setup display one
void setupDisplayOne() {

  if (debugging) {
    Serial.println("Function Called: setupDisplayOne()");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Speed     :");
  lcd.setCursor(0, 1);
  lcd.print("Top Speed :");
  lcd.setCursor(13, 0);
  lcd.print("mph");
  lcd.setCursor(13, 1);
  lcd.print("mph");
}

// Setup display two
void setupDisplayTwo() {

  if (debugging) {
    Serial.println("Function Called: setupDisplayTwo()");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Last Trip :");
  lcd.setCursor(0, 1);
  lcd.print("Total Dist:");
  lcd.setCursor(14, 0);
  lcd.print("mi");
  lcd.setCursor(14, 1);
  lcd.print("mi");
}

// Update display one
void updateDisplayOne() {

  if (debugging) {
    Serial.println("Function Called: updateDisplayOne()");
  }

  // Update Speed
  lcd.setCursor(12, 0);
  lcd.print('0');

  // Update Top Speed
  lcd.setCursor(12, 1);
  lcd.print('0');
  
}

// Update display two
void updateDisplayOne() {

  if (debugging) {
    Serial.println("Function Called: updateDisplayOne()");
  }

  // Update Last Trip
  lcd.setCursor(13, 0);
  lcd.print('0');

  // Update Total Dist
  lcd.setCursor(13, 1);
  lcd.print('0');
  
}

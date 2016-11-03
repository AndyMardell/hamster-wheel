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
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Variables
volatile byte revolutions;
unsigned int rpm;
unsigned long timeold;
const int hallPin = 5; // Hall Effect Sensor on Pin D1

// Debugging
const int debugging = 1;
const int logging = 1;

// Setup
void setup() {

  // Begin Serial
  Serial.begin(115200);

  // Attach interupt to hallPin, call magnetDetect()
  attachInterrupt(hallPin, magnetDetect, RISING);

  // Set up LCD for main program
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RPM:");
  lcd.setCursor(0, 1);
  lcd.print("Revs:");

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
  updateDisplay();
  
}

//This function is called whenever a magnet/interrupt is detected by the arduino
void magnetDetect() {

  if (logging) {
    Serial.println("Detected");
  }

  revolutions++;
  
}

// Update display
void updateDisplay() {

  if (debugging) {
    Serial.println("Function Called: updateDisplay()");
  }

  // Update RPM
  lcd.setCursor(6, 0);
  lcd.print(rpm);

  // Update Revolutions
  lcd.setCursor(7, 1);
  lcd.print(revolutions);
  
}

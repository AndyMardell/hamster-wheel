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
int revolutions;
int rpm;
float speedCmPm;
float milesPerHour;
unsigned long timeold;

// Option Variables
const int radius = 7; // Radius of wheel in cm
const int circumference = 2 * (3.14159265359 * radius);
const int hallPin = 5; // Hall Effect Sensor on Pin D1
const int buttonPin = 4;
int screen = 1; // Screen 1 or 2

// Debugging
const int debugging = 0;
const int logging = 1;

// Setup
void setup() {

  // Begin Serial
  Serial.begin(115200);

  // Attach interupt to hallPin, call magnetDetect()
  attachInterrupt(hallPin, magnetDetect, RISING);

  // Attach interupt to button, call changeDisplay()
  attachInterrupt(buttonPin, changeDisplay, RISING);

  // Set up display
  setupDisplayOne();

  // Reset vars
  revolutions = 0;
  rpm = 0;
  timeold = 0;

  if (logging) {
    Serial.println("Setup Complete");
  }
}

// Main Program
void loop() {

  // Every n revs
  if (revolutions >= 20) { 
    calculateRpm();
    calculateSpeed();
  }

  // Update Display
  if (screen == 1) {
    updateDisplayOne();  
  } else {
    updateDisplayTwo();
  }
}

// Magnet interrupt function
void magnetDetect() {

  revolutions++;

  if (logging) {
    Serial.print("Detected (");
    Serial.print(revolutions);
    Serial.println(")");
  }

  delay(50);
}

// Calculate RPM
void calculateRpm() {
  rpm = 30 * 1000 / (millis() - timeold) * revolutions;
  timeold = millis();
  revolutions = 0;
  
  if (logging) {
    Serial.print("RPM: ");
    Serial.println(rpm); 
  }
}

// Calculate Speed
void calculateSpeed() {

  speedCmPm = circumference * rpm; // cm per minute
  milesPerHour = speedCmPm * 0.000372823; // cm per min to mph

  if (logging) {
    Serial.print("Speed: ");
    Serial.println(milesPerHour); 
  }
}

// Change Display interrupt function
void changeDisplay() {

  if (logging) {
    Serial.println("Function Called: changeDisplay()");
  }

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
void updateDisplayTwo() {

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

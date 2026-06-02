#include <LiquidCrystal.h>

// LCD pins connected to Arduino
const int rs = 13;  // Register select pin
const int en = 12;  // Enable pin
const int d4 = 11;  // Data pin 4
const int d5 = 10;  // Data pin 5
const int d6 = 9;   // Data pin 6
const int d7 = 8;   // Data pin 7

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// XOR gate pin
const int xorPin = 5;

// Capacitor control pins
const int cap420Pin = 4; // 420 µF capacitor control
const int cap1Pin = 1;   // 1 µF capacitor control

// Variables
float pulseWidth = 0;          // Pulse width in microseconds
float phaseDifference = 0.0;   // Phase difference in degrees
float powerFactor = 0.0;       // Power factor

// Constants
const float signalFrequency = 50.0; // Frequency of AC signal in Hz
const float period = 1000000.0 / signalFrequency; // Period in microseconds (e.g., 20000 for 50 Hz)

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin(16, 2); // 16 columns and 2 rows
  lcd.print("Measuring...");
  delay(1000);

  // Set pin modes
  pinMode(xorPin, INPUT);
  pinMode(cap420Pin, OUTPUT);
  pinMode(cap1Pin, OUTPUT);

  // Ensure capacitors are off initially
  digitalWrite(cap420Pin, LOW);
  digitalWrite(cap1Pin, LOW);
}

void loop() {
  // Measure pulse width
  pulseWidth = pulseIn(xorPin, HIGH);

  // Calculate phase difference
  phaseDifference = (pulseWidth * 360.0) / period;

  // Calculate power factor
  powerFactor = cos(radians(phaseDifference));

  // Debugging on Serial Monitor
  Serial.print("Pulse Width: ");
  Serial.print(pulseWidth, 2);
  Serial.print(" us, Phase Angle: ");
  Serial.print(phaseDifference, 2);
  Serial.print(" degrees, Power Factor: ");
  Serial.println(powerFactor, 2);

  // Adjust capacitors to improve power factor
  if (powerFactor < 0.80) {
    digitalWrite(cap420Pin, HIGH); // Engage 420 µF capacitor
    digitalWrite(cap1Pin, HIGH);  // Engage 1 µF capacitor
  } else if (powerFactor < 0.95) {
    digitalWrite(cap420Pin, HIGH); // Engage 420 µF capacitor
    digitalWrite(cap1Pin, LOW);   // Disengage 1 µF capacitor
  } else {
    digitalWrite(cap420Pin, LOW); // Disengage 420 µF capacitor
    digitalWrite(cap1Pin, LOW);  // Disengage 1 µF capacitor
  }

  // Display on LCD
  lcd.clear();
  lcd.print("Pulse: ");
  lcd.print(pulseWidth, 2); // Pulse width in µs
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
  lcd.print(phaseDifference, 2); // Phase angle in degrees
  delay(2000);

  lcd.clear();
  lcd.print("Power Factor:");
  lcd.setCursor(0, 1);
  lcd.print(powerFactor, 2); // Display power factor
  delay(2000);
}

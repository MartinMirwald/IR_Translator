#include <Arduino.h>
#include <tinyNeoPixel.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> // Include for PROGMEM

#define LED_PIN PIN_PA7
#define BUTTON_PIN PIN_PA6
#define IR_LED_PIN PIN_PA3
#define IR_RECEIVER_PIN PIN_PA2
#define NUMPIXELS 1

// Function declarations
void buttonISR();
void sendIRSignal();
void checkIRReceiver();

tinyNeoPixel pixels = tinyNeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
volatile bool buttonPressed = false;
bool irSignalReceived = false;
unsigned long lastIRCheck = 0;
const unsigned long IR_CHECK_INTERVAL = 100; // Check IR every 100ms

void setup() {
  pixels.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(IR_LED_PIN, OUTPUT);
  pinMode(IR_RECEIVER_PIN, INPUT);

  randomSeed(analogRead(0)); // Initialize random seed

  // Enable interrupt on PA6 (falling edge = button press)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  // Set initial color
  uint8_t r = random(0, 51);
  uint8_t g = random(0, 51);
  uint8_t b = random(0, 51);
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();

  // Initialize Serial communication on PA1 (TX)
  Serial.begin(9600); // Set baud rate to 9600
  while (!Serial);    // Wait for Serial to initialize (optional)
}

void loop() {
  // Check for button press
  if (buttonPressed) {
    buttonPressed = false;
    Serial.println(F("Button pressed")); // Log button press

    // Set LED to green
    pixels.setPixelColor(0, pixels.Color(0, 50, 0));
    pixels.show();

    sendIRSignal(); // Send IR signal when button is pressed
  }
  
  // Check IR receiver at regular intervals
  if (millis() - lastIRCheck >= IR_CHECK_INTERVAL) {
    checkIRReceiver();
    lastIRCheck = millis();
  }

  // If IR signal was received, set LED to red
  if (irSignalReceived) {
    Serial.println(F("1"));
    pixels.setPixelColor(0, pixels.Color(50, 0, 0)); // Set LED to red
    pixels.show();
    irSignalReceived = false;
  }
}

void buttonISR() {
  buttonPressed = true;
}

void sendIRSignal() {
  // Simple modulated IR signal (38kHz carrier)
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      digitalWrite(IR_LED_PIN, HIGH);
      delayMicroseconds(13);
      digitalWrite(IR_LED_PIN, LOW);
      delayMicroseconds(13);
    }
    delayMicroseconds(600); // Pause between pulses
  }
}

void checkIRReceiver() {
  if (digitalRead(IR_RECEIVER_PIN) == LOW) {
    delay(5);
    if (digitalRead(IR_RECEIVER_PIN) == LOW) {
      irSignalReceived = true;
    }
  }
}
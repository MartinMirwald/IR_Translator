#include <Arduino.h>
#include <tinyNeoPixel.h>
#include <avr/interrupt.h>

// Debug configuration - Set to 1 to enable debug prints, 0 to disable
#define DEBUG 1

#if DEBUG
#define DEBUG_PRINT(x) Serial1.print(x)
#define DEBUG_PRINTLN(x) Serial1.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#define LED_PIN PIN_PA7
#define BUTTON_PIN PIN_PA6
#define IR_LED_PIN PIN_PA3
#define IR_RECEIVER_PIN PIN_PA2
//#define SERIAL_PIN PIN_PA1
#define NUMPIXELS 1

// Function declarations
void changeColor();
void buttonISR();
void sendIRSignal();
void checkIRReceiver();
void rainbowEffect();

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
  
  #if DEBUG
  // Initialize serial on PA1 for FTDI adapter
  //pinMode(SERIAL_PIN, OUTPUT);
  Serial1.begin(9600);
  DEBUG_PRINTLN(F("Debug mode enabled"));
  DEBUG_PRINTLN(F("IR Translator starting..."));
  #endif

  randomSeed(analogRead(0)); // Initialize random seed

  // Enable interrupt on PA6 (falling edge = button press)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  changeColor();
}

void loop() {
  // Check for button press
  if (buttonPressed) {
    DEBUG_PRINTLN(F("Button pressed"));
    buttonPressed = false;
    changeColor();
    sendIRSignal(); // Send IR signal when button is pressed
  }
  
  // Check IR receiver at regular intervals
  if (millis() - lastIRCheck >= IR_CHECK_INTERVAL) {
    checkIRReceiver();
    lastIRCheck = millis();
  }

  // If IR signal was received, display a rainbow effect
  if (irSignalReceived) {
    DEBUG_PRINTLN(F("Displaying rainbow effect"));
    rainbowEffect();
    irSignalReceived = false;
  }

  //delay(10); // Small delay
}

void changeColor() {
  uint8_t r = random(0, 51);
  uint8_t g = random(0, 51);
  uint8_t b = random(0, 51);
  DEBUG_PRINT(F("Color changed to: R="));
  DEBUG_PRINT(r);
  DEBUG_PRINT(F(" G="));
  DEBUG_PRINT(g);
  DEBUG_PRINT(F(" B="));
  DEBUG_PRINTLN(b);
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
}

// Interrupt Service Routine (ISR)
void buttonISR() {
  buttonPressed = true;
  }

// Send a simple IR signal pattern
void sendIRSignal() {
  DEBUG_PRINTLN(F("Sending IR signal"));
  // Simple modulated IR signal (38kHz carrier)
  for (int i = 0; i < 20; i++) {
    // 38kHz carrier (13µs on, 13µs off)
    for (int j = 0; j < 10; j++) {
      digitalWrite(IR_LED_PIN, HIGH);
      delayMicroseconds(13);
      digitalWrite(IR_LED_PIN, LOW);
      delayMicroseconds(13);
    }
    delayMicroseconds(600); // Pause between pulses
  }
  DEBUG_PRINTLN(F("IR signal sent"));
}

// Check if IR signal is detected
void checkIRReceiver() {
  // Simple IR detection (low signal indicates received IR light)
  if (digitalRead(IR_RECEIVER_PIN) == LOW) {
    // Debounce/verify it's a real signal
    delay(5);
    if (digitalRead(IR_RECEIVER_PIN) == LOW) {
      DEBUG_PRINTLN(F("IR signal received"));
      irSignalReceived = true;
    }
  }
}

// Rainbow color effect when IR is detected
void rainbowEffect() {
  DEBUG_PRINTLN(F("Starting rainbow sequence"));
  // Display a sequence of colors like a mini rainbow
  uint32_t colors[] = {
    pixels.Color(50, 0, 0),   // Red
    pixels.Color(50, 25, 0),  // Orange
    pixels.Color(50, 50, 0),  // Yellow
    pixels.Color(0, 50, 0),   // Green
    pixels.Color(0, 0, 50),   // Blue
    pixels.Color(30, 0, 50)   // Purple
  };
  
  for (int i = 0; i < 6; i++) {
    DEBUG_PRINT(F("Rainbow color: "));
    DEBUG_PRINTLN(i);
    pixels.setPixelColor(0, colors[i]);
    pixels.show();
    delay(100);
  }
  DEBUG_PRINTLN(F("Rainbow sequence completed"));
}
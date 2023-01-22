#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "rgb.h"

#define PIN        2
#define NUMPIXELS  420

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const auto pinOutput = 16;
const auto pinInput = 17;
const auto dayColor = pixels.Color(255, 255, 255);
const auto nightColor = pixels.Color(0, 0, 255);

auto color = dayColor;
auto lastValue = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  pixels.begin();
  pixels.setBrightness(50); // 255

  pinMode(pinOutput, OUTPUT);
  pinMode(pinInput, INPUT_PULLDOWN);

  digitalWrite(pinOutput, HIGH);
  lastDebounceTime = millis();
}

void loop() {  
  auto reading = digitalRead(pinInput);
  
  if (reading != lastValue) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != lastValue) {
        lastValue = reading;
      }
  }

  pixels.clear();
  pixels.fill((reading == LOW) ? dayColor : nightColor, 0, NUMPIXELS);
  pixels.show();
}

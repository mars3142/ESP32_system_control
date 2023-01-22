#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncElegantOTA.h>

#include "rgb.h"

#define PIN        2
#define NUMPIXELS  420

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
AsyncWebServer server(80);

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

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin("SSID", "PASSWORD");
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("SSID");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");

  if (!MDNS.begin("eisenbahn")) {
    Serial.println("Error setting up MDNS responder!");
    delay(10000);
    ESP.restart();
  }  
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

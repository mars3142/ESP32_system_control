#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncElegantOTA.h>

#include "credentials.h"
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

Credentials cred;
int led = 0;

void setup() {
  Serial.begin(115200);

  pixels.begin();
  pixels.setBrightness(50); // 255

  pinMode(pinOutput, OUTPUT);
  pinMode(pinInput, INPUT_PULLDOWN);

  digitalWrite(pinOutput, HIGH);
  lastDebounceTime = millis();

  pinMode(2, OUTPUT);
  for (auto &c : credentials) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(c.ssid.c_str(), c.password.c_str());
    Serial.println("");
    Serial.print("Trying to connect to ");
    Serial.println(c.ssid.c_str());

    // Wait for connection
    unsigned long start = millis();
    while (millis() - start <= 15000) {
      if (WiFi.status() == WL_CONNECTED) {
        break;
      }

      delay(500);
      Serial.print(".");
      led = 1 - led;
      digitalWrite(2, led);
    }

    if (WiFi.status() == WL_CONNECTED) {
      cred = c;
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(cred.ssid.c_str());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  SPIFFS.begin(true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html", false);
  });

  server.serveStatic("/", SPIFFS, "/");

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

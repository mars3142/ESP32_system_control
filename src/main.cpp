#include <Arduino.h>

#include <esp32-hal-log.h>
#include <AccelStepper.h>

#include "connectivity.h"
#include "rgb.h"
#include "server.h"
#include "wled.h"

// Motor Driver
#define stepPin 25
#define dirPin 26

AccelStepper stepper(AccelStepper::FULL2WIRE, stepPin, dirPin);

void setup()
{
  Serial.begin(115200);

  log_d("Total heap: %d", ESP.getHeapSize());
  log_d("Free heap: %d", ESP.getFreeHeap());

  if (psramInit())
  {
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
  }

  xTaskCreatePinnedToCore(
      init_wifi_task, /* Task function. */
      "WiFi",         /* String with name of task. */
      10000,          /* Stack size in bytes. */
      NULL,           /* Parameter passed as input of the task */
      1,              /* Priority of the task. */
      NULL,           /* Task handle. */
      APP_CPU_NUM     /* CPU Core */
  );

  xTaskCreate(
      init_server_task, /* Task function. */
      "Server",         /* String with name of task. */
      10000,            /* Stack size in bytes. */
      NULL,             /* Parameter passed as input of the task */
      1,                /* Priority of the task. */
      NULL              /* Task handle. */
  );

  xTaskCreatePinnedToCore(
      show_wled_task, /* Task function. */
      "WLED",         /* String with name of task. */
      10000,          /* Stack size in bytes. */
      NULL,           /* Parameter passed as input of the task */
      1,              /* Priority of the task. */
      NULL,           /* Task handle. */
      APP_CPU_NUM     /* CPU Core */
  );

  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(50);
  stepper.setSpeed(200);
  stepper.moveTo(200);
}

void loop()
{
  /// nothing to do here
}

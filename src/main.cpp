#include <Arduino.h>

#include <esp32-hal-log.h>
#include <AccelStepper.h>

#include "connectivity.h"
#include "server.h"
#include "wled.h"

// Motor Driver
#define step_pin 25
#define dir_pin 26

AccelStepper stepper(AccelStepper::FULL2WIRE, step_pin, dir_pin);

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
      initWifiTask, // Task function.
      "WiFi",       // String with name of task.
      5000,         // Stack size in bytes.
      NULL,         // Parameter passed as input of the task
      1,            // Priority of the task.
      NULL,         // Task handle.
      APP_CPU_NUM   // CPU Core
  );

  xTaskCreatePinnedToCore(
      initServerTask, // Task function.
      "Server",       // String with name of task.
      10000,          // Stack size in bytes.
      NULL,           // Parameter passed as input of the task
      1,              // Priority of the task.
      NULL,           // Task handle.
      APP_CPU_NUM     // CPU Core
  );

  xTaskCreatePinnedToCore(
      showWledTask, // Task function.
      "WLED",       // String with name of task.
      10000,        // Stack size in bytes.
      NULL,         // Parameter passed as input of the task
      1,            // Priority of the task.
      NULL,         // Task handle.
      APP_CPU_NUM   // CPU Core
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

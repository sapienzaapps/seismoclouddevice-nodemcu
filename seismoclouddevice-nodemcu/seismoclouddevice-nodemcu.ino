
#include "common.h"

unsigned long debug_lastms = 0;

// Initialize device
void setup() {
#ifdef DEBUG
  delay(1000);
  // start serial port for debug
  Serial.begin(115200);
#endif

  LED_init();

  Debug(F("SeismoCloud-Arduino version "));
  Debugln(VERSION);

  // Power up and calibrate the seismometer
  Debugln(F("Init seismometer and calibrate"));
  LED_accel_calibr();
  seismometerInit();

  // Initialize NodeMCU WiFi
  LED_wait_net_cfg();
  NodeMCU_init();

  LED_connection();
  // Connect to API server
  if (!apiConnect()) {
    LED_lost_api();
    soft_restart();
  }

  Debugln(F("Boot completed"));
  Debugln();
  LED_startup_blink();
  LED_ready();
}

void loop() {
  // Execute API events
  apiTick();

  // Execute seismometer events
  seismometerTick();
}

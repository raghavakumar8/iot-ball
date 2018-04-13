/*
 * File: websocket.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include "scrum-tracker/include/scrum_tracker.h"

uint64_t last_send_time = 0;
 
void setup()
{
  Serial.begin(115200);

  setupLEDS();
  setupIMU();
  setupWIFI();

  // Indicate successful setup
  circle(0, 255, 0, 1, 50, true);
  circle(0, 255, 0, 1, 50, false);
}

void loop() {
  updateIMU();
  if (throwDetected())
  {
    flash(0, 0, 255, 100);
  }

  if (shakeDetected())
  {
    flash(255, 0, 0, 100);
  }

  webSocket.loop();

  if(connected)
  {
    uint64_t now = millis();
    if (now - last_send_time > SEND_INTERVAL_MS)
    {
      Serial.println("Sending info!");
      webSocket.sendTXT("blah blah");

      last_send_time = now;
    }
  }
}
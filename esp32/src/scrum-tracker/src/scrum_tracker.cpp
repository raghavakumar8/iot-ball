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
  flash(0, 0, 255, 100);
  delay(1000);
  pulsate(255, 0, 0, 1000);
  delay(1000);
  circle(255, 0, 0, 3, 50, true);
  circle(0, 255, 0, 3, 50, false);
  delay(1000);

  WiFi.begin("ssid", "password");

  int ledState = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    digitalWrite(5, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin("192.168.1.220", 80, "/echo");
  webSocket.onEvent(webSocketEvent);
}

void loop() {
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
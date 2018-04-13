/*
 * File: websocket.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include "scrum-tracker/include/scrum_tracker.h"

WebSocketsClient webSocket;

bool connected = false;

void setupWIFI()
{
  WiFi.begin("ssid", "password");
  Serial.print("Attempting to connect to WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    pulsate(0, 0, 255, 1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  webSocket.begin("192.168.1.220", 80, "/echo");
  webSocket.onEvent(webSocketEvent);
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("Web socket disconnected.");
      connected = false;
      break;
    case WStype_CONNECTED:
      Serial.printf("Connected to: %s\n",  payload);
      connected = true;
      break;
    case WStype_TEXT:
      Serial.printf("Received reply: %s\n", payload);
      break;
    case WStype_BIN:
      Serial.println("Received unexpected binary data.");
      break;
  }
}


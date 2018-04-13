/*
 * File: websocket.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include "scrum-tracker/include/scrum_tracker.h"

#define WIFI_TIMEOUT 10000

WebSocketsClient webSocket;

bool connected = false;

void setupWIFI()
{
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  Serial.printf("Attempting to connect to %s\n", WIFI_SSID);

  uint64_t start = millis(); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    if(millis() - start > WIFI_TIMEOUT)
    {
      Serial.println("\nFailed to connect to WiFi.");
      flashFatalError();
    }

    pulsate(0, 0, 255, 1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  webSocket.begin(SERVER, 80, "/echo");
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


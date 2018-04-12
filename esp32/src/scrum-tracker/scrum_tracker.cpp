/*
 * File: websocket.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include <Arduino.h>
#include <WebSocketsClient.h>
#include <Adafruit_NeoPixel.h>

#define SEND_INTERVAL_MS 1000

// Declare library objects
WebSocketsClient webSocket;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(10, 15);
 
uint64_t last_send_time = 0;
bool connected = false;
 
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
 
void setup()
{
  Serial.begin(115200);

  leds.begin();
  leds.show();

  while(1)
  {
    for (int i = 0; i < 10; i++)
    {
      Serial.println(i);
      leds.setPixelColor(i, 0, 0, 255);
      leds.show();
      delay(100);
    }
  }

  Serial.println("This should never print");
  
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
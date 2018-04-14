/*
 * File: WebConnection.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include "WebConnection.h"

using namespace std::placeholders;

void WebConnection::initialize(String ssid, String pwd, String server, 
                               int port, String ns, int timeout)
{
  WiFi.begin(ssid.c_str(), pwd.c_str());
  Serial.printf("Attempting to connect to %s", WIFI_SSID);

  uint64_t start = millis();
  while (WiFi.status() != WL_CONNECTED) 
  {
    if(millis() - start > timeout)
    {
      Serial.println("\nFailed to connect to WiFi.");
      leds_.fatalError(3000);
    }

    leds_.pulse(CRGB::Blue, 1000);
    Serial.print(".");
  }

  Serial.println(" done!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  client_.begin(server, port, ns);
  client_.onEvent(std::bind(&WebConnection::webSocketEvent, this,  _1, _2, _3));
}

bool WebConnection::send(String text)
{
  if(!connected_)
  {
    return false;
  }

  client_.sendTXT(text);
  return true;
}

void WebConnection::update()
{
  client_.loop();
}

void WebConnection::webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("Web socket disconnected");
      connected_ = false;
      break;
    case WStype_CONNECTED:
      Serial.printf("Connected to: %s\n",  payload);
      connected_ = true;
      break;
    case WStype_TEXT:
      Serial.printf("Received reply: %s\n", payload);
      if (payload[0] == 'O' && payload[1] == 'K')
      {
        leds_.pulse(CRGB::Green, 500);
      }
      break;
    case WStype_BIN:
      Serial.println("Received unexpected binary data");
      break;
  }
}


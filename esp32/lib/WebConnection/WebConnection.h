/*
 * File: WebConnection.h
 * Author: Raghava Kumar
 */

#ifndef WEB_CONNECTION_H
#define WEB_CONNECTION_H

#include <WebSocketsClient.h>
#include <LEDRing.h>

class WebConnection
{
public:
  WebConnection(LEDRing& leds)
   : leds_(leds)
   , connected_(false)
  {}
  ~WebConnection(){}

  void initialize(String ssid, String pwd, String server, int port, String ns, int timeout);
  bool send(String text);
  void update();

private:
  void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
  WebSocketsClient client_;
  LEDRing& leds_;

  bool connected_;
};

#endif // WEB_CONNECTION_H
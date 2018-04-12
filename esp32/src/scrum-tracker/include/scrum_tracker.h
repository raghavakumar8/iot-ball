/*
 * File: scrum_tracker.h
 * Author: Raghava Kumar
 */

#include <Arduino.h>
#include <MPU9250.h>
#include <WebSocketsClient.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_COUNT 10
#define NEOPIXEL_PIN 15

#define SEND_INTERVAL_MS 1000

extern WebSocketsClient webSocket;
extern Adafruit_NeoPixel leds;
extern MPU9250 imu;

extern void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
extern bool connected;

extern void setupLEDS();
extern void flash(uint8_t r, uint8_t g, uint8_t b, int ms);
extern void pulsate(uint8_t r, uint8_t g, uint8_t b, int ms);
extern void circle(uint8_t r, uint8_t g, uint8_t b, int times, int ms, bool direction);

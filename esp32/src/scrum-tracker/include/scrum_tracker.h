/*
 * File: scrum_tracker.h
 * Author: Raghava Kumar
 */

#include <Arduino.h>
#include <WebSocketsClient.h>
#include <Adafruit_NeoPixel.h>
#include <SparkFunMPU9250-DMP.h>

#define NEOPIXEL_COUNT 10
#define NEOPIXEL_PIN 15

#define SEND_INTERVAL_MS 10000

extern WebSocketsClient webSocket;
extern Adafruit_NeoPixel leds;
extern MPU9250_DMP imu;

extern void setupWIFI();
extern void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
extern bool connected;

extern void setupLEDS();
extern void flash(uint8_t r, uint8_t g, uint8_t b, int ms);
extern void pulsate(uint8_t r, uint8_t g, uint8_t b, int ms);
extern void circle(uint8_t r, uint8_t g, uint8_t b, int times, int ms, bool direction);
extern void flashFatalError();

extern void setupIMU();
extern void updateIMU();
extern bool throwDetected();
extern bool shakeDetected();

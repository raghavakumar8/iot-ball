/*
 * File: websocket.cpp
 * Author: Raghava Kumar
 * Reference: https://platformio.org/lib/show/549/WebSockets/examples
 */

#include <Arduino.h>
#include <LEDRing.h>
#include <MotionTracker.h>
#include <WebConnection.h>

LEDRing leds(10);
MotionTracker motion_tracker;
WebConnection web_connection(leds);

// Scrum data
bool scrum_started = false;
uint64_t scrum_start_time, section_start_time;
int current_section;

uint64_t section_time[100];
char scrum_data[500];

void setup()
{
  Serial.begin(115200);

  leds.initialize<15>();
  motion_tracker.initialize();
  web_connection.initialize(WIFI_SSID, WIFI_PWD, 
                     SERVER, 80, "/echo", 10000);

  // Indicate successful setup
  leds.run(CRGB::Green, 1, 50, true);
  leds.run(CRGB::Green, 1, 50, false);
}

void loop()
{
  Motion detected = motion_tracker.detect();
  web_connection.update();

  uint64_t now = millis();
  if (scrum_started)
  {
    if (detected == Motion::SHAKE)
    {
      section_time[current_section] = now - section_start_time;
      uint64_t total_scrum_time = now - scrum_start_time;

      Serial.printf("Total scrum time: %ld\n", total_scrum_time);

      if (total_scrum_time < 10000)
      {
        Serial.println("This is too short to be a real meeting. Skipping!");

        leds.flash(CRGB::Red, 150);
        delay(150);
        leds.flash(CRGB::Red, 150);
      }
      else
      {
        memset(scrum_data, 0, 500);
        sprintf(scrum_data, "%d", current_section+1);
        sprintf(scrum_data + strlen(scrum_data), ", %ld", total_scrum_time);
        for(int i = 0; i <= current_section; i++)
        {
          sprintf(scrum_data + strlen(scrum_data), ", %ld", section_time[i]);
        }
        Serial.printf("Sending: %s\n", scrum_data);
        web_connection.send(scrum_data);

        leds.pulse(CRGB::Green, 500);
      }

      scrum_started = false;
    }
    else if (detected == Motion::THROW)
    {
      section_time[current_section] = now - section_start_time;
      leds.flash(CRGB::Yellow, 100);

      section_start_time = millis();
      current_section++;
    }
    else if (detected == Motion::TWIST)
    {
      leds.run(CRGB::Green, 1, 50, false);
      leds.run(CRGB::Green, 1, 50, true);
      leds.run(CRGB::Green, 1, 50, false);
      leds.randomRun(CRGB::Green, 50);
    }
  }
  else
  {
    if (detected == Motion::SHAKE)
    {
      leds.flash(CRGB::Blue, 150);
      delay(150);
      leds.flash(CRGB::Blue, 150);

      scrum_started = true;
      scrum_start_time = millis();
      section_start_time = millis();
      current_section = 0;
    }
  }

  delay(10);
}
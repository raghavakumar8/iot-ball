# iot-ball

Sparkfun ESP32 Thing + IMU + Adafruit Neopixels = ❤️

## ESP32 Setup
- Clone this repository
```
git clone https://github.com/raghavakumar8/iot-ball.git
```
- Install PlatformIO
```
pip install -U platformio
```
- Fix a known ESP32 I2C issue by patching some files
  - Download patch from the .zip mentioned [here](https://github.com/espressif/arduino-esp32/issues/741)
  - Follow instructions from [here](https://github.com/stickbreaker/arduino-esp32/issues/16)

- Compile and upload
```
cd iot-ball/esp32
pio run -e blink -t upload
```

---
title: DSTIKE ESP32 Watch
date: 2022-02-02
draft: false
---

![](assets/Pasted%20image%2020220202163610.png)

Sold as the **dstike esp32 watch devkit**, this is an ESP32 microcontroller with an OLED display, a NeoPixel, flashlight, buzzer, battery, charging circuit, SD card slot, and a few buttons.

There are a few different versions, some in this watch form factor, some as single boards, different screens, ESP8266 etc etc.   Searching for *DSTIKE* should bring up some results.  These boards were designed for hacking WiFi networks, executing deauth attacks and such.

I'm interested in using the device as an OSC logger (and perhaps later playback) device, for capturing streaming OSC messages in on-body applications.

As of writing this, I have the device receiving a predefined OSC message and displaying it on the screen.

[💾 download](assets/esp-watch-demo.ino)

# ARDUINO SET UP
This is especially for the DSTIKE esp32 OLED device.
Set up testing environment as below.


## 1. Install the environment for ESP32 using board manager
details: https://github.com/espressif/arduino-esp32
 
 
## 2. Install OLED library
Search for "ESP8266 and ESP32 Oled Driver for SSD1306 display" in library manager

more details: https://github.com/ThingPulse/esp8266-oled-ssd1306


## 3. Install Adafruit NeoPixel Library
Search for "Adafruit Neopixel" in library manager
More details: https://github.com/adafruit/Adafruit_NeoPixel
 

## 4.Code Example for oled version 
esp-watch-demo.ino in this folder

https://github.com/lspoplove/D-duino-project/blob/master/ESP32WatchDev/ESP32WatchTest.ino

(my modified version of this sketch is available to download above)

---


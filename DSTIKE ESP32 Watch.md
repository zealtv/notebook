---
title: DSTIKE ESP32 Watch
date: 2022-02-02
lastmod: 2022-02-03T14:49:47+11:00
draft: false
---

![](assets/Pasted%20image%2020220202163610.png)

Sold as the **dstike esp32 watch devkit**, this is an ESP32 microcontroller with an OLED display, a NeoPixel, flashlight, buzzer, battery, charging circuit, SD card slot, and a few buttons.

There are a few different versions, some in this watch form factor, some as single boards, different screens, ESP8266 etc etc.   Searching for *DSTIKE* should bring up some results.  These boards were designed for hacking WiFi networks, executing deauth attacks and such.

I'm interested in using the device as an OSC logger (and perhaps later playback) device, for capturing streaming OSC messages in on-body applications.

I am developing an arduino application to achieve this. In its current state, the sketch will receive all messages on port 9000.  It will iterate over an array if it exists, and  any integers, floats, or doubles will be converted to floats and displayed on the screen.  If there are more than 3 elements in the array, they will not be visible.

### usage
- Follow onscreen instructions to connect to network.
- OSC messages are received on port 9000 and displayed on screen.
- Press and hold front-bottom-left button during boot to disconnect from current WiFi network.

### Required libraries
- OSC https://github.com/CNMAT/OSC
- Autoconnect https://github.com/hieromon/AutoConnect
- OLED manager https://github.com/ThingPulse/esp8266-oled-ssd1306
- NeoPixel https://github.com/adafruit/Adafruit_NeoPixel

#### [💾 download](assets/esp-watch-demo/esp-watch-demo.ino)

🔺🔷🔻🔷🔺🔷🔻🔷🔺🔷🔻🔷🔺🔷🔻🔷🔺

### Future steps
-  press button to open file on SD card.
-  write incoming OSC messages to SD card.
-  press button to close file on SD card.
-  display filename and recording state on screen
-  indicate if device is receiving data, and other information via NeoPixel

If you are starting developing from scratch with one of these watches, read on...

---


# Setting up the initial development environment
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

## 5. Build settings
![](assets/Pasted%20image%2020220202173518.png)


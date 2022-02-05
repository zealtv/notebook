---
title: packetPunk - OSC logging on an ESP32 Watch
date: 2022-02-02
lastmod: 2022-02-05T18:51:12+11:00
draft: false
---

![](assets/Pasted%20image%2020220202163610.png)

Sold as the **dstike esp32 watch devkit**, this is an ESP32 microcontroller with an OLED display, a NeoPixel, flashlight, buzzer, battery, charging circuit, SD card slot, and a few buttons.

There are a few different versions, some in this watch form factor, some as single boards, different screens, ESP8266 etc etc.   Searching for *DSTIKE* should bring up some results.  These boards were designed for hacking WiFi networks, executing deauth attacks and such.

I'm interested in using the device as an OSC logger (and perhaps later playback) device, for capturing streaming OSC messages in on-body applications.

I am developing an arduino application to achieve this. In its current state, the sketch will receive all messages on port 9000.  The inbuilt neoPixel indicates current mode, incoming data, and acts as a sync indicator when starting to record.



### usage
- Follow onscreen instructions to connect to network (password is packetPunk)
- OSC messages are received on port 9000.
- Press the top left button to start logging, press again to save file.
- Long-press  bottom left button to disconnect from current WiFi network.
- If you need to manage saved networks use [this sketch](https://github.com/Hieromon/AutoConnect/blob/master/examples/Credential/Credential.ino).

### Required libraries
- OSC https://github.com/CNMAT/OSC
- Autoconnect https://github.com/hieromon/AutoConnect
- OLED manager https://github.com/ThingPulse/esp8266-oled-ssd1306
- NeoPixel https://github.com/adafruit/Adafruit_NeoPixel
- AceButton https://github.com/bxparks/AceButton


#### [💾 download](assets/packetpunk/packetpunk.ino)

Example log playback patches for Max and Pure Data available [here](assets/packetpunk/packetpunkplayback.zip).

🔺🔷🔻🔷🔺🔷🔻🔷🔺🔷🔻🔷🔺🔷🔻🔷🔺

### Future steps
- Better dynamic WiFi management (update state if knocked off network etc)
- dynamic display ( if it can be efficient)
- playback (maybe someday)

---

If you are starting development from scratch with one of these watches, read on...

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

## 6. SD info
This is a particularly useful [sketch outlining SD card file procedures](https://github.com/espressif/arduino-esp32/blob/master/libraries/SD_MMC/examples/SDMMC_Test/SDMMC_Test.ino) 
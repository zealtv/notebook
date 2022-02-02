---
title: portable video streaming approaches for art installations
date: 2022-01-31
lastmod: 2022-02-02T22:08:17+11:00
---

I am surveying small, locally streaming cameras for applications in art installations such as capturing video to process, present, or to use for tracking, analysis, or interaction.

![](assets/Pasted%20image%2020220129143605.png)
Recently I tested the [Nextech mini wifi camera](NEXTECH%20QC3868.md)


[This is a good example of a Raspberry pi approach](https://www.reddit.com/r/raspberry_pi/comments/ppu00m/prototype_low_latency_wireless_streaming_camera/) that focusses on low latency.  This would be particularly helpful when using for tracking. The Nextech camera

The system above uses [UV4L](https://www.linux-projects.org/uv4l/) which looks like an excellent tool kit.  It has a feature that allows you to capture an HDMI output and stream that.  So you could for example make an [openframeworks](http://openframeworks.cc) application using processing a video camera feed and then forward out the fullscreen output of that application as a low latency MJPG stream.

Another approach would be to send video via NDI with [Dicaffeinate](https://dicaffeine.com/). This approach would be my preference due to the ease of integrating NDI in other frameworks and video tools - TouchDesigner, OpenFrameworks etc.  
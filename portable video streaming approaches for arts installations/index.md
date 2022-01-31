---
title: portable video streaming approaches for art installations
when: 
- 2022-01-31
draft: "true"
---


I am surveying small, locally streaming cameras for applications in art installations such as capturing video to process, present, or to use for tracking, analysis, or interation.

Recently I tested the [Nextech mini wifi camera](../NEXTECH%20QC3868/index.md)

[This is a good example of a Raspberry pi approach](https://www.reddit.com/r/raspberry_pi/comments/ppu00m/prototype_low_latency_wireless_streaming_camera/) that focusses on low lactency.  This would be particularly helpful when using for tracking. The Nextech camera

The system above uses [UV4L](https://www.linux-projects.org/uv4l/) which looks like an excellent tool kit.  It has a headture that allows you to capture an HDMI output and stream that.  So you could for example make an [openframeworks](openframeworks.cc) application using processing a video camera feed and then forward out the fullscreen output of that application as a low latency MJPG stream.
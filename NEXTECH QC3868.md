---
title: NEXTECH QC3868
date: 2022-01-31
lastmod: 2022-02-02T23:06:06+11:00
---


![](assets/Pasted%20image%2020220129143605.png)



The NEXTECH QC3863 ([available here](https://www.jaycar.com.au/miniature-1080p-wifi-ip-camera-with-rechargeable-battery-and-ir-leds-for-night-vision/p/QC3863)) is a small, battery powered, IP security camera, capable of recording to an internal SD card and being administrated via a mobile app.  Streaming video is able to be accessed via local network, though it is poorly documented.  This thing is insecure as shit.  Keep it off the internet and untrusted networks as much as possible.

#### Steps to access local streaming video
- Use Tuya Smart iOS app to connect camera to wifi
- you can disable the timestamp while you are in here
- Find or set cameras IP address

You can then access the following:
(my camera IP was set to 192.168.1.120)

- A (very basic) web interface:

[http://192.168.1.120:9527/index.htm](http://192.168.1.120:9527/index.htm)

- Two high resolution H264 streams, high latency (1000ms +) 

[rtsp://admin:admin@192.168.1.120:6554/stream_0](rtsp://admin:admin@192.168.1.120:6554/stream_0)
[rtsp://admin:admin@192.168.1.120:6554/stream_1](rtsp://admin:admin@192.168.1.120:6554/stream_1)

- A MJPG ( i think?) stream.  640x360, low latency (approx 300ms)

[http://admin:admin@192.168.1.120:9527/videostream.cgi?user=admin&pwd=admin&resolution=32](http://admin:admin@192.168.1.120:9527/videostream.cgi?user=admin&pwd=admin&resolution=32)

- Single JPG frames, similar to above

[http://admin:admin@192.168.1.120:9527/snapshot.cgi?user=admin&pwd=admin](http://admin:admin@192.168.1.120:9527/snapshot.cgi?user=admin&pwd=admin)


![](assets/Pasted%20image%2020220129212255.png)

RTSP in TouchDesigner could be a bit of a game changer in cases where latency isn't critical.


The camera's IR sensitivity interferes with colours. It's not a faithful representation but has a good vibe when giving you colour.

![](assets/Pasted%20image%2020220129232525.png)


## Specs
- Operating Temperature : -10.000000°C, 60.00000000°C
- Storage Media Type : microSD
- Maximum Capacity : 64.00000000GB
- Wireless Standard : 802.11b, 802.11g, 802.11n
- Length : 33.00000000mm
- Width : 33.00000000mm
- Height : 33.00000000mm
- Weight : 36.00000000g
- PSU Powers : Camera
- DC Plugpack Voltage : 5.00000000V
- DC Current Draw : 1.00000000A
- Suitable PSU Included : true
- DG UN Number : 3481.00000000
- DG Ship Name : 3481 - Lithium Ion Batteries Packed In Equipment
- DG Class : 9.00000000
- DG Pack Group : II - Medium Danger
- DG Hazardous Chemical Code : 4W
- Battery Watt Hour Rating (Required for Lithium Batteries) : 2.96000000Wh
- Dangerous Road Freight : false
- Dangerous Air Freight : true
- Type of Camera : Miniature
- Output Signal : Digital Wireless
- Resolution : 1080p (1920 x 1080)
- In-built Microphone : false
- Lens Diameter : 2.40000000mm
- Lens Angle : 138.00000000°
- Focal Length of cam/lens : 2.90000000mm, 2.90000000mm
- Frame Rate : 10.00000000fps, 25.00000000fps
- Infrared Enabled : true
- Infrared Range : 5.00000000m
- Number of LEDs : 4.00000000pc
- Apps to suit : Apple iOS, Android

---

## Testing notes 🐉🐉🐉🐉🐉🐉


![](assets/Pasted%20image%2020220129175619.png)

Camera battery arrived very flat.  

On boot the camera created an open network.  Windows did not easily connect.  Instead, I used a (very polished) 3rd party app called **Tuya Smart**.

- 👍 Camera is capable of simultaneous charging and operating
- 👍 Camera connected easily  
- 👍 Video quality is good
- 👎 Latency is high on default camera settings and  my medium strength network

Tuya Smart turns out to be better suited to home automation than camera configuration.

Camera presented open network on reboot and iOS connected successfully.
Network information reports router @ **192.168.66.1**
Phone has been given  **192.168.66.100**
Subnet mask is 255.255.255.0

Trying to connect to router via browser returns ERR_CONNECTION_FAILED 

The device has remembered the network and reconnects on boot.

I can ping the camera.

Using the BlueCAM search tool from the www.bluecam.cloud found in the spec sheet, I was able to see this:

![](assets/Pasted%20image%2020220129193822.png)
 ![](assets/Pasted%20image%2020220129193858.png)

Revealing RTSP streams!

THIS ADDRESS GAVE ME AN ADMIN SCREEN
http://192.168.66.1:9527/index.htm

```
user: admin
pass: admin
```

![](assets/Pasted%20image%2020220129200015.png)

The firefox stream viewer looks 720p.  Lacenty is good.  Deals with low light well.

![](assets/Pasted%20image%2020220129200135.png)
![](assets/Pasted%20image%2020220129200218.png)

No instantly apparent configuration settings for joining WiFi.
Would ideally like to join WiFi without broadcasting a stream to a third party.

This link gets us the image directly in browser, although defaulted to 640x360
http://192.168.66.1:9527/videostream.cgi?loginuse=admin&loginpas=admin

Returning to Tuya Smart to connect camera to local network.
Pairing light goes solid after viewing QR code.
Gave the camera a static local ip in router and rebooted camera.
Camera reconnected with reserved ip.

![](assets/Pasted%20image%2020220129202750.png)

Progress.

And we have the full res stream in VLC!

![](assets/Pasted%20image%2020220129203154.png)

[rtsp://admin:admin@192.168.1.120:6554/stream_0](rtsp://admin:admin@192.168.1.120:6554/stream_0)

[rtsp://admin:admin@192.168.1.120:6554/stream_1](rtsp://admin:admin@192.168.1.120:6554/stream_1)

Both streams have significant latency.  The stream shown in the browser is very snappy, however.  I suspect this is mjpg as that was mentioned in the spec sheet.

From the served admin page - the firefox link is making the call:
```
javascript:signin_serverpush()
```

Poking around further in the website served by the camera, the Wifi SSID and password are there in plain text.  Not great security.  It would be wise to assume anything these cameras see may as well be public.  

in the contents of **get_ipc_status.cgi**
```
var sdtotal=0;
var sdfree=0;
var alias="IPCAM";
var sys_ver="21.36.0.7.16E";
var kernelversion="Linux version 3.10.14";
var deviceid="xwlz7e81101376b01d12";
var mac="00:C0:01:A2:00:01";
var wifimac="40:aa:56:d8:b0:8e";
var alarm_status=0;
var dns_status=0;
var upnp_status=0;
var sdstatus=0;
var record_sd_status=0;
var externwifi=1;
var devicetype=0;
var awb=1;
var learnstatus=0;
var batcapacity=1517;
var batstatus=4;
var pirtimes=0;
var pirjpeg=0;
var pirrecord=0;
var messagerecord=0;
var mcuversion=0;
var linda_is_sleeping=0;
var fn_4g=0;
var fn_cloud_record=0;
var fn_rtmp=0;
var fn_mqtt=0;
var fn_custom1=1;
var fn_custom2=1;
var fn_custom3=0;
var fn_custom4=1;
var fn_custom5=0;
var fn_custom6=0;
var signal_level_wifi=83;
var signal_level_sim=0;
var linked_internet=0;
var sim_type=0;
var sim_iccid="";
```
The variable linda_is_sleping is pretty odd.  Searching the string does not throw anything up on google.  Nor Github.  Weird. 


FOUND THE STREAM
[http://admin:admin@192.168.1.120:9527/videostream.cgi?user=admin&pwd=admin&resolution=32](http://admin:admin@192.168.1.120:9527/videostream.cgi?user=admin&pwd=admin&resolution=32)

JPG snapshots are available here:
[http://admin:admin@192.168.1.120:9527/snapshot.cgi?user=admin&pwd=admin](http://admin:admin@192.168.1.120:9527/snapshot.cgi?user=admin&pwd=admin)

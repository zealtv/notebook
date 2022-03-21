---
title: notes on OSC in Pure Data
date: 2022-02-26
lastmod: 2022-02-26T21:37:15+11:00
draft: true
---

Working on my PhD project, I am needing to send and receive telemetry data as OSC.  While doing this I've noticed I am losing precision in my latitude and longitude readings.  I have these  stored as floating point values with 7 decimals of accuracy, though after parsing them in PD I am down to 4 decimal points.  I suspect the OSCParse object is to blame here.

using mrpeaches \[udpreceive] object and \[unpackOSC] objects does not solve the problem.  Interrogating the data further - it seems there are 6 decimals of precision inclusive of each side of the decimal point.

From PD:
```
/telemetry 769200 0 0 0 0 -36.7158 147.125 247.353 0.080056 -0.474286 -0.673482 -0.561307 332.31 0 0 0 -6.37432 2.02017 7.93358 0.338606 -0.203012 -0.125433
```

Max does not have the same problem...

From Max:
```
print: /telemetry 762100 0 0. 0. 0. -36.716118 147.12587 243.215607 0.078466 -0.280268 -0.517904 -0.804406 18.77 0. 0. 0. -6.472389 3.246001 6.845042 -0.020667 0.080673 -0.34683

```

ok - it looks as though this occuring because of the use of 32bit (single precision) floats, which makes sense - although it means max is doing some tricky business under the hood to preserve precision when sending OSC internally.  I am now looking into the (frankly amazing) odot package to see if it can help.



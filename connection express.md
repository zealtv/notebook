---
title: Connection Express
date: 2022-01-31
draft: true
---
## Interactive Train System

## System

#### Tram
* 2-4 Cameras
* Speed controller
* Pi for wifi control
* Powerbank or batter
* Charge controller
* 3D printed carriage
* Dock for charging


#### Track
* Wooden track (ikea)
* 3D printed options for bridges, forks etc


#### Viewing and control system
* Computer with 2-4 video outputs
* Router


### Tasks
	
1.A) Prototype Carriage
- Docking station
- Charging 
- Wifi Control

1.B) Test cameras
- determine camera choice
 
1.C) Develop control system

2.) 
- Revise prototype 
- integrate chosen cameras
- Configure control system
- test and debug

3.)
Install

``` mermaid
flowchart  
direction TB

subgraph network  
 direction TB
 
 subgraph control computer
 vs(video software)
 cs(control software) -.-> pi
 end
 
 subgraph carriage  w/ independant wifi cameras
 cc[charge controller]
 pi[pi zero]  
 bat[battery]
 sc[speed controller]
 mot[motor]
 ir[input rails]
 ds[docking station detection switch]
 ds --- pi
 ir --- cc
 sc --- mot
 pi --- sc
 bat --- sc
 bat --- pi
 cc --- bat
 bat --- cam1
 bat --- cam2
 bat --- cam3
 bat --- cam4
 end  



 cam1 -.-> vs
 cam2 -.-> vs
 cam3 -.-> vs
 cam4 -.-> vs
 
end

vs ---> d1(display 1)
vs ---> d2(display 2)
vs ---> d3(display 3)
vs ---> d4(display 4)

```

****
# ESPSatTracker (work still in progress, ideas wellcommend)

This project is intended for tracking HAM radio satellites using stock available cheep components and 3D printer. 
* Antena is 5+8 elem Yagi from [dk7zb](https://www.qsl.net/dk7zb/Duoband/5+8_2m-70cm.htm)
* Rotator was heavily inspired from [tysonpower.de](https://tysonpower.de/blog/diy-az-el-antenna-rotator-for-under-150)

## Features
It shall be remotely operated (Bluetooth), it shall bear heavier antennas (preparing for moonscating), it shall be portable, it shall bring fun.

## Parts used
* 2000 x 15 x 15 allu rectangle tube for antenna beam
* two 2000 x 20 x 20 allu rectangle tube for rotator frame and azimuth axe
* ?x 8mm allu tubes for yagi elements
* 1x 12mm allu tube for yagi radiator
* plenty of M4 screws with nuts and waschers
* 4mm plywood for rotator sides
* 2x TB6600 stepper drivers
* 2x NEMA24 stepper motors
* 2 endstops from eBay (optional and not really used)
* Wemos Lolin32 as rotator controller (other ESP32 development board should do too)
* PLA filanment (ABS was bending too much for big parts)

## Software used
* OpenSCAD & Slic3r & RepetierHost for 3d modelling and printing
* [platformio] (https://platformio.org) for SW build
* gpredict for satellite tracking
* STL and scad projects and fotos are at [thingiverse](https://www.thingiverse.com/thing:4246844)

## Build
### Dependencies
Install platformio and libraries Streaming and ESP8266_SSD1306
```
platformio lib --global install Streaming
platformio lib --global install ESP8266_SSD1306
```
### Build and upload:
Run in the RotatorController directory
```
platformio run -t upload
```

### Start
1. Without power, orient the rotator to the north and level antena horizontally.
1. Power up motors and ESP32 controller
1. In your bluetooth search for a device named ESP32 Rotator and add that device.
1. In the terminal connect to it `sudo rfcomm connect 1 <device mac address>`
1. In another terminal window start a rotator control daemon `rotctld -r /dev/rfcomm1 -s 115200 -m 603` The 603 is a rotator model 603, 'GS-232B'
1. Then in gpredict add a new rotator. Choose some name, host is `localhost`, port: `4533`, Az type: `0 - 180 - 360`, Max Az:`360`, Max El: `180`

## Notes
The rotator can commands like speed and park to zero. However the rotctld nor it's stand-allone application are using them. Just stop, move to azimuth and elevation, asking for current azimuth and elevation is actually used by the daemon.  

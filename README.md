# ESPSatTracker (work still in progress, ideas wellcommend)

This project is intended for tracking HAM radio satellites using stock available cheep components and 3D printer. 
* Antena is 5+8 elem Yagi from dk7zb [https://www.qsl.net/dk7zb/Duoband/5+8_2m-70cm.htm]
* Rotator was heavily inspired from [https://tysonpower.de/blog/diy-az-el-antenna-rotator-for-under-150]

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
* 2 endstops from eBay
* Wemos Lolin32 as rotator controller
* PLA filanment (ABS was bending too much for big parts)

## Software used
* OpenSCAD & Slic3r & RepetierHost for 3d modelling and printing
* platformio [https://platformio.org] for SW build
* gpredict for satellite tracking

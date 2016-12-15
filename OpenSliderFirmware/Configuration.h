/*
     ___                    __    __   _____  ___  __  __
    /___\_ __   ___ _ __   / _\  / /   \_   \/   \/__\/__\
   //  // '_ \ / _ \ '_ \  \ \  / /     / /\/ /\ /_\ / \//
  / \_//| |_) |  __/ | | | _\ \/ /___/\/ /_/ /_///__/ _  \
  \___/ | .__/ \___|_| |_| \__/\____/\____/___,'\__/\/ \_/
      |_|

   Configuration.h
   Written by Greg Deeth for the Arduino Mega 2560 + RAMPS v1.4
   Copyright (C) 2016 Open Slider Firmware https://sourceforge.net/projects/open-slider/

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//Choose your language
#include "Language_EN.h"

//Settings
#define BAUDRATE                115200    //Serial baud rate

#define enableBeeps             true      //Turn LCD beeps on or off (true/false)

boolean Loop        =           true;     //When endswitch is hit, reverse direction?

//Stepper Configuration
#define STEP_MODE_4             4000      //Threshold for shifting to 4 steps per loop.
#define STEP_MODE_2             2000      //Threshold for shifting to 2 steps per loop.
#define Slide_SPR               200       //Full steps per revolution
#define X_SPR                   200       //Please enter the # of steps required for 1 full rotation @ full step mode 1:1
#define Y_SPR                   200

#define MICROSTEP_S             1         //Microstep set on board drivers
#define MICROSTEP_P             1         //(1, 2, 4, 8, 16) Please enter the microstep setting on the drivers in multiplier number
#define MICROSTEP_T             1         //Example: 1/16 or 1:16 would be 16.  1/1 or 1:1 would be 1.

#define Slide_maxSpeed          8000      //Steps per second
#define X_maxSpeed              8000
#define Y_maxSpeed              8000

#define DEFAULT_SPEED_S         7500      //Steps per second
#define DEFAULT_SPEED_P         4000
#define DEFAULT_SPEED_T         4000

/*
     ___                    __    __   _____  ___  __  __
    /___\_ __   ___ _ __   / _\  / /   \_   \/   \/__\/__\
   //  // '_ \ / _ \ '_ \  \ \  / /     / /\/ /\ /_\ / \//
  / \_//| |_) |  __/ | | | _\ \/ /___/\/ /_/ /_///__/ _  \
  \___/ | .__/ \___|_| |_| \__/\____/\____/___,'\__/\/ \_/
      |_|

   pins_RAMPS.h
   Modified by Greg Deeth for Open Slider
   Copyright (C) 2016 Open Slider Firmware https://sourceforge.net/projects/open-slider/

   Some elements based on Marlin 3D Printer Firmware
   Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]

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

#include <LiquidCrystal.h>

#define SERVO1_PIN          6
#define SERVO2_PIN          5
#define SERVO3_PIN          4

#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_MIN_PIN          18
#define Z_MAX_PIN          19
#define Z_MIN_PROBE_PIN    32

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define SDSS               53
#define LED_PIN            13

// Use the RAMPS 1.4 Analog input 5 on the AUX2 connector
#define FILWIDTH_PIN        5 // ANALOG NUMBERING

// define digital pin 4 for the filament runout sensor. Use the RAMPS 1.4 digital input 4 on the servos connector
#define FIL_RUNOUT_PIN      4

#define PS_ON_PIN          12
#define TEMP_0_PIN         13   // ANALOG NUMBERING
#define TEMP_1_PIN         15   // ANALOG NUMBERING
#define TEMP_BED_PIN       14   // ANALOG NUMBERING

//Smart Adapter LCD (common non-graphic LCD)
#define LCD_BEEP           37
#define LCD_RS             16
#define LCD_ENABLE         17
#define LCD_D4             23
#define LCD_D5             25
#define LCD_D6             27
#define LCD_D7             29
#define LCD_EN1            31
#define LCD_EN2            33
#define LCD_ENC            35
#define LCD_BTN            41
#define LCD_SDCARDDETECT   49

//Setup Pins
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void initializePins() {
  //Set LCD resolution
  lcd.begin(20, 4);
  
  //Set up the encoder and button on the LCD
  pinMode(LCD_EN1, INPUT_PULLUP);   //Knob pin 1
  pinMode(LCD_EN2, INPUT_PULLUP);   //Knob pin 2
  pinMode(LCD_ENC, INPUT_PULLUP);   //Knob push to click
  pinMode(LCD_BTN, INPUT_PULLUP);   //Kill switch
  pinMode(LCD_BEEP, OUTPUT);        //LCD buzzer
  
  //Set stepper pins
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  pinMode(E1_STEP_PIN, OUTPUT);   //Slide
  pinMode(E1_DIR_PIN, OUTPUT);
  pinMode(E1_ENABLE_PIN, OUTPUT);  

  //Set Max Min pins (end switches)
  pinMode(X_MAX_PIN, INPUT);
  pinMode(X_MIN_PIN, INPUT);
  pinMode(Y_MAX_PIN, INPUT);
  pinMode(Y_MIN_PIN, INPUT);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);
  pinMode(Z_MIN_PIN, INPUT_PULLUP);
}

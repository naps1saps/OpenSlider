/*
     ___                    __    __   _____  ___  __  __
    /___\_ __   ___ _ __   / _\  / /   \_   \/   \/__\/__\
   //  // '_ \ / _ \ '_ \  \ \  / /     / /\/ /\ /_\ / \//
  / \_//| |_) |  __/ | | | _\ \/ /___/\/ /_/ /_///__/ _  \
  \___/ | .__/ \___|_| |_| \__/\____/\____/___,'\__/\/ \_/
      |_|

   OpenSliderFirmware - Main
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

#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include <SoftwareSerial.h>
#include "AccelStepper.h"
#include "pins_RAMPS.h"
//#include "configuration.h"
#include "Funcions.h"
//#include "Stepper.h"
//#include "menu.h"

SoftwareSerial mySerial(0, 1); // RX, TX

unsigned int g_isDisplaying = false;
long uptimeTracker = 0;
int programMode = 0;          // 0-LIVE  ELSE-TIME LAPSE

boolean stringComplete = false;




/////////////////////////////////////////////////////////
//SETUP SETUP SETUP SETUP SETUP
/////////////////////////////////////////////////////////
void setup() {
  printAbout();
  //delay(3000);
  initializePins();
  initializeSteppers();
  initializeSerial();
  setupMenus();
  enableSteppers();

  //DO THIS LAST.  Initialize Timers: 1,000,000ms = 1sec.
  //Timer1.initialize(51);                    //Priority Loop (Steppers, motors, ...)
  //Timer1.attachInterrupt(ISR_stepperManager);
  //Timer3.initialize(100);                    //Priority Loop (Steppers, motors, ...)
  //Timer3.attachInterrupt(ISR_serial);

}

//////////////////////////////////////////////////////////
//LOOP LOOP LOOP LOOP LOOP
//////////////////////////////////////////////////////////
void loop() {
  //Program Modes
  if (programMode == 0) {
    LIVE_stepperManager();
    checkEndSwitches();
  } else {
    printScreen();
    getUserInputs();
    checkEndSwitches();
  }

  String incomingString = "";
  if (Serial.available() > 0)
  {
    incomingString = Serial.readString();
    Serial.println(incomingString);
  }

  if (incomingString == "u") {
    lcdMenuManager.DoMenuAction( MENU_ACTION_UP );
  } else if (incomingString == "d") {
    lcdMenuManager.DoMenuAction( MENU_ACTION_DOWN );
  } else if (incomingString == "s") {
    if ( g_isDisplaying )
    {
      g_isDisplaying = false;
      lcdMenuManager.DrawMenu();
    }
    else
    {
      lcdMenuManager.DoMenuAction( MENU_ACTION_SELECT );
    }
  } else if (incomingString == "b") {
    lcdMenuManager.DoMenuAction( MENU_ACTION_BACK );
  } else if (incomingString == "l") {
    Loop = !Loop;
  } else if (incomingString == "e") {
    Slide.setSpeed(2000);
  } else if (incomingString.startsWith("slidespeed")) {
    Slide.setSpeed(100);
    Slide.setSpeed(incomingString.substring(incomingString.indexOf(":") + 1).toInt());
  }

  //If the current rotary switch position has changed then update everything
  if (virtualPosition != lastCount) {
    //Write out to LCD
    if (virtualPosition < lastCount) {
      lcdMenuManager.DoMenuAction(MENU_ACTION_UP);
    } else {
      lcdMenuManager.DoMenuAction(MENU_ACTION_DOWN);
    }
    lastCount = virtualPosition;
  }
}

void initializeSerial() {
  Serial.begin(BAUDRATE);
  Serial.println("OPEN SLIDER Written by Greg Deeth");
  Serial.println("Copyright 2016 under the GNU License");
  //mySerial.begin(115200);
  //mySerial.println("Hello, world?");
}

/*void readSerial() {
  int i;
  int length = Serial.available();
  int string_buffer[32];

  //copy data out of the receive buffer
  for (i = 0; i < length; i++) {
    string_buffer[i] = Serial.read();
  }

  //run a string compare function, or otherwise parse the received data
  //Serial.println(string_buffer);
  }*/

unsigned long screen_previousMillis = 0;
void printScreen() {
  unsigned long currentMillis = millis();   //Capture time
  if ((currentMillis - screen_previousMillis) >= 250) {
    lcd.setCursor(13, 3);
    lcd.write(byte(0));
    lcd.print((millis() - uptimeTracker) / 1000);
    screen_previousMillis = currentMillis;
  }
}



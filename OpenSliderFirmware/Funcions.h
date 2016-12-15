/*
     ___                    __    __   _____  ___  __  __
    /___\_ __   ___ _ __   / _\  / /   \_   \/   \/__\/__\
   //  // '_ \ / _ \ '_ \  \ \  / /     / /\/ /\ /_\ / \//
  / \_//| |_) |  __/ | | | _\ \/ /___/\/ /_/ /_///__/ _  \
  \___/ | .__/ \___|_| |_| \__/\____/\____/___,'\__/\/ \_/
      |_|

   Functions.h
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

#include "configuration.h"
#include "menu.h"

//Define functions from Stepper.h
void enableSteppers();
void disableSteppers();

//Rotary encoder position trackers
int lastCount = 50;
volatile int virtualPosition = 50;
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev = 0;

unsigned long previousMillis = 0;
unsigned long beep_previousMillis = 0;




//Interrupt Timer1
void getUserInputs() {
  unsigned long currentMillis = millis();   //Capture time

  encoder_A = digitalRead(LCD_EN2);    // Read encoder pins
  encoder_B = digitalRead(LCD_EN1);

  if ((!encoder_A) && (encoder_A_prev)) {
    if (encoder_B) {
      virtualPosition--;
    } else if (!encoder_B) {
      virtualPosition++;
    }
  }
  encoder_A_prev = encoder_A;

  //Reset value
  if (virtualPosition <= 1 || virtualPosition >= 99) {
    virtualPosition = 50;
  }

  //LCD_ENC Rotary Button Action
  if (!digitalRead(LCD_ENC) && (currentMillis - previousMillis > 500)) {
    lcdMenuManager.DoMenuAction(MENU_ACTION_SELECT);    //Select action for the menu
    if (enableBeeps) {
      digitalWrite(LCD_BEEP, HIGH);
      beep_previousMillis = currentMillis;
    }
    enableSteppers();
    previousMillis = currentMillis;
  }

  //LCD_BTN Kill Button Action
  if (!digitalRead(LCD_BTN) && (currentMillis - previousMillis > 500)) {
    if (enableBeeps) {
      digitalWrite(LCD_BEEP, HIGH);
      disableSteppers();
      beep_previousMillis = currentMillis;
    }
    previousMillis = currentMillis;
  }

  //Stop Beep
  if (currentMillis - beep_previousMillis > 300) {
    digitalWrite(LCD_BEEP, LOW);
  }
}

void lcdPrint(int row, int col, String message) {
  lcd.setCursor(col, row);
  lcd.print(message);
}

boolean slideAtEnd = false;
void checkEndSwitches() {
  if ((digitalRead(Z_MAX_PIN) == LOW) && (digitalRead(Z_MIN_PIN) == LOW)) {
    Slide.setSpeed(0);
    slideAtEnd = true;
  } else if ((digitalRead(Z_MAX_PIN) == LOW) || (digitalRead(Z_MIN_PIN) == LOW)) {
    //digitalWrite(LED_PIN, HIGH);
    if (!slideAtEnd) {
      slideAtEnd = true;
      if (loop) {
        Slide.setSpeed(-(Slide.speed()));
      } else {
        Slide.setSpeed(0);
      }
    }
  } else if(slideAtEnd == true){
    slideAtEnd = false;
  }
}


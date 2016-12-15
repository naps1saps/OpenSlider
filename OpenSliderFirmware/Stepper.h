/*
     ___                    __    __   _____  ___  __  __
    /___\_ __   ___ _ __   / _\  / /   \_   \/   \/__\/__\
   //  // '_ \ / _ \ '_ \  \ \  / /     / /\/ /\ /_\ / \//
  / \_//| |_) |  __/ | | | _\ \/ /___/\/ /_/ /_///__/ _  \
  \___/ | .__/ \___|_| |_| \__/\____/\____/___,'\__/\/ \_/
      |_|

   Stepper.h
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

//#include <AccelStepper.h>

//Create stepper objects
AccelStepper Slide(1, E1_STEP_PIN, E1_DIR_PIN);
AccelStepper Xaxis(1, X_STEP_PIN, X_DIR_PIN);
AccelStepper Yaxis(1, Y_STEP_PIN, Y_DIR_PIN);

void initializeSteppers() {
  Slide.setMaxSpeed(Slide_maxSpeed);
  Slide.setSpeed(DEFAULT_SPEED_S);
  Slide.setEnablePin(E1_ENABLE_PIN);
  Slide.setPinsInverted(false, false, true);

  Xaxis.setMaxSpeed(X_maxSpeed);
  Xaxis.setSpeed(DEFAULT_SPEED_P);
  Xaxis.setEnablePin(X_ENABLE_PIN);
  Xaxis.setPinsInverted(false, false, true);

  Yaxis.setMaxSpeed(Y_maxSpeed);
  Yaxis.setSpeed(DEFAULT_SPEED_T);
  Yaxis.setEnablePin(Y_ENABLE_PIN);
  Yaxis.setPinsInverted(false, false, true);
}

//Power on steppers
void enableSteppers() {
  Slide.enableOutputs();
  Xaxis.enableOutputs();
  Yaxis.enableOutputs();
}

//Power off steppers
void disableSteppers() {
  Slide.disableOutputs();
  Xaxis.disableOutputs();
  Yaxis.disableOutputs();
}

//Interrupt Timer1
void ISR_stepperManager() {
  Slide.runSpeed();
  Xaxis.runSpeed();
  Yaxis.runSpeed();
}

//MENU->Start
void startProgram(char* pMenuText, void *pUserData) {
  Serial.println(pMenuText);
  /*switch (pMenuText) {
    case "Basic Motion":   //Basic Motion
      lcd.print("SUCCESS");
      break;
    case "2":   //Timelapse
      break;
    case "3":   //Focus Stacking
      break;
    }*/
}

void takeStep(int STEPPER) {
  digitalWrite(STEPPER, HIGH);
  //count++;  //placed here to act as a small delay to prevent missed steps by the driver
  digitalWrite(STEPPER, LOW);
}

//////////////////////////////////////////////////////////////////
//  LIVE CONTROL  Uses different methods for controling movement.
//  All Arrays = {S, P, T}
//////////////////////////////////////////////////////////////////
unsigned long previousMicros[] = {0, 0, 0};
int runningStepRate[] = {0, 0, 0};
int stepRate[] = {DEFAULT_SPEED_S, DEFAULT_SPEED_P, DEFAULT_SPEED_T};
int stepMode[] = {0, 0, 0};
void LIVE_stepperManager() {
  // move mode change to serial receive
  if (stepRate[0] >= STEP_MODE_4) {
    stepMode[0] = 4;
    runningStepRate[0] = stepRate[0] / 4;
  } else  if (stepRate[0] >= STEP_MODE_2) {
    stepMode[0] = 2;
    runningStepRate[0] = stepRate[0] / 2;
  } else   {
    stepMode[0] = 1;
    runningStepRate[0] = stepRate[0];
  }

  if (stepRate[1] >= STEP_MODE_4) {
    stepMode[1] = 4;
    runningStepRate[1] = stepRate[1] / 4;
  } else if (stepRate[1] >= STEP_MODE_2) {
    stepMode[1] = 2;
    runningStepRate[1] = stepRate[1] / 2;
  } else {
    stepMode[1] = 1;
    runningStepRate[1] = stepRate[1];
  }

  if (stepRate[2] >= STEP_MODE_4) {
    stepMode[2] = 4;
    runningStepRate[2] = stepRate[2] / 4;
  } else if (stepRate[2] >= STEP_MODE_2) {
    stepMode[2] = 2;
    runningStepRate[2] = stepRate[2] / 2;
  } else {
    stepMode[2] = 1;
    runningStepRate[2] = stepRate[2];
  }

  unsigned long currentMicros = micros();   //Capture time
  if ((currentMicros - previousMicros[0]) >= (1000000 / runningStepRate[0] )) {
    for (int i = 1; i <= stepMode[0]; i++) {
      takeStep(E1_STEP_PIN);
    }
    previousMicros[0] = currentMicros;
  }

  if ((currentMicros - previousMicros[1]) >= (1000000 / runningStepRate[1] )) {
    for (int i = 1; i <= stepMode[1]; i++) {
      takeStep(X_STEP_PIN);
    }
    previousMicros[1] = currentMicros;
  }

  if ((currentMicros - previousMicros[2]) >= (1000000 / runningStepRate[2] )) {
    for (int i = 1; i <= stepMode[2]; i++) {
      takeStep(Y_STEP_PIN);
    }
    previousMicros[2] = currentMicros;
  }
}


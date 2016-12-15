/*
     ___                    __    __   _____  ___  __  __
    /___\_ __   ___ _ __   / _\  / /   \_   \/   \/__\/__\
   //  // '_ \ / _ \ '_ \  \ \  / /     / /\/ /\ /_\ / \//
  / \_//| |_) |  __/ | | | _\ \/ /___/\/ /_/ /_///__/ _  \
  \___/ | .__/ \___|_| |_| \__/\____/\____/___,'\__/\/ \_/
      |_|

   Menu.h
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

#include "MenuEntry.h"
#include "MenuLCD.h"
#include "MenuManager.h"
#include "Stepper.h"

MenuLCD lcdMenu(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 20, 2);
MenuManager lcdMenuManager( &lcdMenu);//pass the g_menuLCD object to the g_menuManager with the & operator.

//Custom LCD character blocks
byte upArrow[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
};


//setupMenus
//This function is called during setup to populate the menu with the tree of nodes
//This can be a bit brain-bending to write.  If you draw a tree you want for your menu first
// this code can be a little easier to write.  Add the nodes in a depth-first order for
// the easiest code and least amount of temporary variables.
// http://en.wikipedia.org/wiki/Depth-first
// The MenuManager code is the same for building the menu as for selecting it via inputs.
// You create the menu entries and then move about the menu structure using Up, Down, Select as if you
// were selecting them via the inputs, and then use either AddChild or Add sibling from the selected node
// to create your menu.
//
//  This sample code is a simple stopwatch.  Our menu will look like this:
//  Stopwatch
//  |-Start
//  |-Stop
//  |-Reset
//  Timer
//  |-Set Time
//  |-AutoReset
//  | |-On
//  | |-Off
//  |-Start
//  |-Stop
//  Credits

void setupMenus()
{
  lcdMenu.MenuLCDSetup();  
  //Add nodes via a depth-first traversal order
  MenuEntry * p_menuEntryRoot;
  //Add root node
  
  //MenuEntry( const char * menuText, void * userData/*=0* /, MENU_ACTION_CALLBACK_FUNC func);

  ////////////BASIC MOTION/////////////
  p_menuEntryRoot = new MenuEntry("Basic Motion", NULL, NULL);
  lcdMenuManager.addMenuRoot( p_menuEntryRoot );
  lcdMenuManager.addChild( new MenuEntry("Speed", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Dampening", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Loop?", NULL, NULL ) );
  lcdMenuManager.addChild( new MenuEntry("TEST RUN", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("START", NULL, startProgram) );
  lcdMenuManager.addChild( new MenuEntry("STOP", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("< Back", (void *) &lcdMenuManager, MenuEntry_BackCallbackFunc) );

  ////////////TIME LAPSE/////////////
  lcdMenuManager.SelectRoot();
  lcdMenuManager.addSibling( new MenuEntry("Time Lapse", NULL, NULL ) );
  lcdMenuManager.MenuDown();
  lcdMenuManager.MenuSelect();
  lcdMenuManager.addChild( new MenuEntry("Slider Settings", NULL, NULL) );
  lcdMenuManager.MenuSelect();
  lcdMenuManager.addChild( new MenuEntry("Distance", NULL, NULL ) );
  lcdMenuManager.addChild( new MenuEntry("Speed", NULL, NULL ) );
  lcdMenuManager.addChild( new MenuEntry("Dampening", NULL, NULL ) );
  lcdMenuManager.addChild( new MenuEntry("< Back", (void *) &lcdMenuManager, MenuEntry_BackCallbackFunc) );
  lcdMenuManager.addSibling( new MenuEntry("Camera Settings", NULL, NULL) );
  lcdMenuManager.MenuDown();
  lcdMenuManager.addChild( new MenuEntry("Shutter Speed", NULL, NULL ) );
  lcdMenuManager.addChild( new MenuEntry("Shutter Buffer", NULL, NULL ) );
  lcdMenuManager.addChild( new MenuEntry("Mv > Shoot > Mv?", NULL, NULL ) );
  lcdMenuManager.addChild( new MenuEntry("< Back", (void *) &lcdMenuManager, MenuEntry_BackCallbackFunc) );
  lcdMenuManager.addSibling( new MenuEntry("Loop?", NULL, NULL ) );
  lcdMenuManager.addSibling( new MenuEntry("TEST RUN", NULL, NULL) );
  lcdMenuManager.addSibling( new MenuEntry("START", NULL, NULL) );
  lcdMenuManager.addSibling( new MenuEntry("STOP", NULL, NULL) );
  lcdMenuManager.addSibling( new MenuEntry("< Back", (void *) &lcdMenuManager, MenuEntry_BackCallbackFunc) );

  ////////////FOCUS STACKER/////////////
  lcdMenuManager.SelectRoot();
  lcdMenuManager.addSibling( new MenuEntry("Focus Stacker", NULL, NULL ) );
  lcdMenuManager.MenuDown();    //Move down one to add Focus Stacker ^
  lcdMenuManager.MenuDown();
  lcdMenuManager.addChild( new MenuEntry("Distance", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("# Shots to Take", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Shutter Speed", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("TEST RUN", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("START", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("STOP", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("< Back", (void *) &lcdMenuManager, MenuEntry_BackCallbackFunc) );

  ////////////STOP MOTION/////////////
  lcdMenuManager.SelectRoot();
  lcdMenuManager.addSibling( new MenuEntry("Stop Motion", NULL, NULL ) );
  lcdMenuManager.MenuDown();    //Move down one to add Focus Stacker ^
  lcdMenuManager.MenuDown();
  lcdMenuManager.MenuDown();
  lcdMenuManager.addChild( new MenuEntry("Frames/s", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Distance", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Real Time to Pos", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Shutter Speed", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("TEST RUN", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("START", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Click -M-> NEXT", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("<-M- PREVIOUS", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("< Back", (void *) &lcdMenuManager, MenuEntry_BackCallbackFunc) );

  ////////////HARDWARE SETTINGS////////////////
  lcdMenuManager.SelectRoot();
  lcdMenuManager.addSibling( new MenuEntry("SETUP", NULL, NULL) );
  lcdMenuManager.MenuDown();
  lcdMenuManager.MenuDown();
  lcdMenuManager.MenuDown();
  lcdMenuManager.MenuDown();
  lcdMenuManager.addChild( new MenuEntry("Steps/rev.", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Step Multiplier", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Maximum Steps/s", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Drive Gear # Teeth", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("Frames/s", NULL, NULL) );
  lcdMenuManager.addChild( new MenuEntry("< Back", (void *) &lcdMenuManager, MenuEntry_BackCallbackFunc) );

  lcdMenuManager.SelectRoot();    //Highlight the top most item when device boots
  lcdMenuManager.DrawMenu();      //Draw the menu

  //Initialize custom LCD character blocks
  lcd.createChar(0, upArrow);
}

void printAbout(){
  //Loading Screen
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("OPEN SLIDER");
  lcd.setCursor(3, 1);
  lcd.print("By Greg Deeth");
  lcd.setCursor(0, 3);
  lcd.print("Version 0.0");
}


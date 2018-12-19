/*
 * BootMenu on serial port.
 * 
 * The boot menu can read/write EEPROM settings
 * 
 * To enter the menu, 
 * press ENTER at the first second after boot.
 * 
 * You can leave the menu with "EXIT" command.
 * For more information see "HELP".
 * 
 * Created: nlga 2015.12.11
 * 
 */
 
#include "bootmenu.h"



String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

bool EnterBootMenu(void) {
    bool enterTOMenu=false;
    inputString.reserve(100);

    Serial.begin(115200);
    Serial.print(F("\r\nPress ENTER to run BootMenu!"));
            
    for(int i=0; i < 3 ;i++) {
      if (Serial.available()) {
        char inChar = (char)Serial.read();
        if (inChar==13) {
          enterTOMenu=true;
          break;
        }
      }
      Serial.print(".");
      delay(1000);
    }

    if (enterTOMenu==true) {
      Serial.print(F("OK\r\n"));
      RunBootMenu();
    } else {
      Serial.print(F("MISSED\r\n"));
    }
  
}


void RunBootMenu(void) {
  char string[32];
  //TVarAssociation var1(F("sd"),1,INT8);
  //var.string = F("asdf");
  /*
   * 
   while(1) {
    ReadCommand();
    DispatchCommand();
    
    
  }
  */
  
}






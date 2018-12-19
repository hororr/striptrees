#ifndef BOOTMENU_H
#define BOOTMENU_H
#include "Arduino.h"


// only ADD for this list!
enum VARTYPE { INT8, INT16, INT32, CHAR, STRING };

// put variable associations here
struct TVarAssociation {
  const __FlashStringHelper * string;  // referenced as ...
  uint16_t pointer;
  VARTYPE type; 
  TVarAssociation() {}
  TVarAssociation(const __FlashStringHelper * _string,uint16_t _pointer,VARTYPE _type) {
    string = _string;
    pointer = _pointer;
    type = _type;
  }
};












bool EnterBootMenu(void) ;


#endif

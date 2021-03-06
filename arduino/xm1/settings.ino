
#include "settings.h"
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>

struct StoreStruct storage;

/*
 *  FOR DEFAULT SETTINGS FILL THIS
 */
void LoadDefaultSettings(void) {
 storage.a = 220;


}


void loadSettings() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2])
    for (unsigned int t=0; t<sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
}

void saveSettings() {
  for (unsigned int t=0; t<sizeof(storage); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
}




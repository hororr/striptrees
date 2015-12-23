#ifndef SETTINGS_H
#define SETTINGS_H

/*
 *  Add new variable:
 *  1-add to StoreStruct
 *  2-add default value to LoadDefaultSettings()
 *  3-add new version number
 *  
 */
 
// ID of the settings block
#define CONFIG_VERSION "v01"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

struct StoreStruct {
  // This is for mere detection if they are your settings
  char version[4];
  // The variables of your settings
  int a;
};

extern struct StoreStruct storage;

void loadSettings();
void saveSettings();









#endif


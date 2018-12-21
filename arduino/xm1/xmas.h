#ifndef XMAS_H
#define XMAS_H
#include "Arduino.h"
#include "circularbuffer.h"

#define MYDEBUGSERIAL 0

#define NUMPIXELS      300
#define MAX_FPS 60

enum currentProgramType { PROG_AUTO=50, PROG_TEST=51, PROG_MANUAL=52, PROG_COMM=53 };
enum networkCommands { CMD_RAW24=3 , CMD_RAW24_2 = 4, CMD_PGM_START=20, CMD_SET_FPS=60 };
//#define CMD_SET_FPS     60
//#define CMD_RAW24       3

struct  stripArrayStruct{
  byte ch[900];
} ;

extern currentProgramType currentProgram;
extern unsigned long global_timer;
extern CircularBuffer <stripArrayStruct,16> stripArray;
extern unsigned long lastUDPPacketReceived;
extern void ChangeProgram(currentProgramType _pt);
extern bool ModeChanged;
extern unsigned int currentFps;

void Wifi_Setup();
void Wifi_HandleClients();

void Led_Init();
void Led_Show();
void Led_Clear();
void Led_setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

void led_clear(void);
boolean fade(int n);
void copy_b(void);
void shift_up(void);
void shift_dn(void);
void rnd_add(uint8_t m);
void rnd_clear(uint8_t m);
void colorwheel(int n,int pos,int br);
void setPixel1b(int n, uint8_t br,uint8_t r, uint8_t g, uint8_t b);
void setPixel1(int n, uint8_t r, uint8_t g, uint8_t b);
boolean rndStart(int n);

void updatePrg(void);
void startPrg(void);
int getcPrg(void);
void nextPrg(void);

int ParseUDP(void);


uint8_t * getPixelPointer();

#endif

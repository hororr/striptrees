#include "xmas.h"


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            12



class LedStrip {
  private:
    
  public:
};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t * shadow1 = 0;
uint8_t * shadow2 = 0;

uint8_t * getPixelPointer() {
  return pixels.getPixels();
}
void setPixel1b(int n, uint8_t br,uint8_t r, uint8_t g, uint8_t b) {
  uint8_t * p = &shadow1[3*n];
 p[1] = (r * br)>>8;
 p[0] = (g * br)>>8;
 p[2] = (b * br)>>8;
}

void setPixel1(int n, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t * p = &shadow1[3*n];
  p[1] = r;
  p[0] = g;
  p[2] = b;
}

uint8_t getR1(int n) {
  uint8_t * p = &shadow1[3*n];
  return p[1];
}

uint8_t getG1(int n) {
  uint8_t * p = &shadow1[3*n];
  return p[0];
}

uint8_t getB1(int n) {
  uint8_t * p = &shadow1[3*n];
  return p[2];
}

uint8_t getR2(int n) {
  uint8_t * p = &shadow2[3*n];
  return p[1];
}

uint8_t getG2(int n) {
  uint8_t * p = &shadow2[3*n];
  return p[0];
}

uint8_t getB2(int n) {
  uint8_t * p = &shadow2[3*n];
  return p[2];
}

void Led_Init() {
  pixels.begin();
  shadow1 = (uint8_t *)malloc(3 * NUMPIXELS);
  shadow2 = (uint8_t *)malloc(3 * NUMPIXELS);
}

void Led_Clear() {
  memset(getPixelPointer(), 0, NUMPIXELS*3 );
}
void Led_Show() {
  pixels.show();
}

void colorwheel(int n,int pos, int br=255) {
  int r,g,b;
  pos = 255 - pos;
  if (pos < 85) { 
        r=255 - pos * 3;
        g=0;
        b=pos * 3;
        }  else 
          if (pos < 170) { 
                          pos -= 85; 
                          r=0;
                          g=pos * 3;
                          b=255 - pos * 3;
                        }  else { 
                                  pos -= 170;
                                  r=pos * 3;
                                  g=255 - pos * 3;
                                  b=0;
                                }
   r=(r*br) >> 8;
   g=(g*br) >> 8;
   b=(b*br) >> 8;
   setPixel1(n,r,g,b);
}

void rnd_clear(uint8_t m) {
    for(uint8_t i=0;i<m;i++) {
      setPixel1(random(NUMPIXELS),0,0,0);
    }
}

void rnd_add(uint8_t m) {
  for(uint8_t i=0;i<m;i++) {
    setPixel1(random(NUMPIXELS),0,random(200),0);
  }
}

void shift_up() {
  for(int i=NUMPIXELS-1;i>0;i--) {
    int r = getR1(i-1);
    int g = getG1(i-1);
    int b = getB1(i-1);
    setPixel1(i,r,g,b);
  }
  setPixel1(0,0,0,0);
}

void shift_dn() {
  for(int i=0;i<NUMPIXELS-1;i++) {
    int r = getR1(i+1);
    int g = getG1(i+1);
    int b = getB1(i+1);
    setPixel1(i,r,g,b);
  }
  setPixel1(NUMPIXELS-1,0,0,0);
}

void copy_b(void) {
    memcpy(pixels.getPixels(), shadow1 , 3*NUMPIXELS); 
}

unsigned long fadetimer = 0;

boolean fade(int n) { 
  float factor = 2.0; 
  if (fadetimer == 0) { 
    fadetimer = global_timer; 
    memcpy(shadow2,pixels.getPixels(), 3*NUMPIXELS); 
  }
  
  factor = float(global_timer - fadetimer)/(float)n; 
  if (factor <= 1.0) { 
      for(int j=0;j<NUMPIXELS;j++) { 
        int r = getR2(j) + (getR1(j) - getR2(j))*factor;
        int g = getG2(j) + (getG1(j) - getG2(j))*factor;
        int b = getB2(j) + (getB1(j) - getB2(j))*factor;
        pixels.setPixelColor(j,r,g,b);
      } 
      return false; 
    }
    fadetimer = 0;
    return true; 
} 

boolean rndStart(int n) {
  int start = (int)random(100);
  return (start > n);
}

void led_clear(void) {
 memset(shadow1,0,NUMPIXELS*3);
}


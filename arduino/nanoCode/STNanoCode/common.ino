#include "common.h"

void toggle_led(void) {
  static bool led_on=true;
  if (led_on) {
    digitalWrite(13, HIGH);       // sets the digital pin 13 on
    led_on=false;
  } else {
    digitalWrite(13, LOW);       // sets the digital pin 13 off
    led_on=true;
  }
}

void led_on() {
  digitalWrite(13, HIGH);       // sets the digital pin 13 on
}
void led_off() {
  digitalWrite(13, LOW);       // sets the digital pin 13 off
}

#include "xmas.h"

//#include <WiFiUdp.h>




unsigned long global_timer = 0;
unsigned long next_update = 0;
unsigned long next_prg = 0;
unsigned int currentFps=20;
bool isRunning=false; // manual mode is running
boolean nextProg = false;

// set back to AUTO MODE
const unsigned int idleTimeToAutoMODE=30000;
unsigned long lastUDPPacketReceived=0;
currentProgramType currentProgram=PROG_AUTO;
bool ModeChanged;
//bool automaticModeEnabled=false;



void setup() {
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  //ESP.wdtDisable();

  EnterBootMenu();
  
  Led_Init();
  Wifi_Setup();
  global_timer = millis();
  next_update = global_timer + 1000; //16;
  next_prg = global_timer + 360000; // 5 percentként új program
  
  startPrg();
  
  Serial1.begin(38400); 
  Serial.write("Started\r\n");
 /*
    Serial1.write("SOF           ~   ~   ~EOF");
    delay(1000);
    Serial1.write("SOF           ~   ~~  ~EOF");
    delay(1000);
*/

}
//extern WiFiUDP udp;
//extern WiFiUDP udp2;
stripArrayStruct sa;

void ChangeProgram(currentProgramType _pt) {
  if (currentProgram != _pt) {
    currentProgram = _pt;
    Led_Clear();
    Led_Show();
    ModeChanged = true;
  }
}

void loop() {
  global_timer = millis();
  ModeChanged = false;
  
  // DECIDE the MODE
  if (currentProgram != PROG_AUTO) {
    //if time elapsed, switch back to auto
    if ( global_timer > (lastUDPPacketReceived + idleTimeToAutoMODE) ) {
        ChangeProgram(PROG_AUTO);
        Serial.println("AUTO-MODE-TIME");    delay(100);
    }
  }


  if (currentProgram == PROG_AUTO) {      
    // AUTO MODE
    Wifi_HandleClients();
    ParseUDP(currentProgram);
    if (!ModeChanged) {
    
        if ((global_timer > next_prg) ||(nextProg)  ) { 
          nextProg = false;
          next_prg = global_timer + 360000;
          nextPrg();
        }
    
        if ( global_timer > next_update ) {
         next_update = global_timer + (1000/currentFps); //32; // 16=60Hz 32=30Hz
         updatePrg();  
         Led_Show();
        } 
    }
    
  } else  if (currentProgram == PROG_MANUAL) {
      ParseUDP(currentProgram);
      if (!ModeChanged) {
          if (global_timer > next_update ) {
            bool canDisplay=false;
            //Serial.println("time\r\n");
            //delay(1000);
            while(global_timer > next_update) {
              // pop data packets
              
              if (!isRunning) { // if it stopped now
                if (stripArray.remain() < stripArray.Half) {  // still waiting for more packets
                  //isRunning = false; //useless
                  canDisplay = false;
                } else {  // lets go!
                  isRunning = true;
                  canDisplay = true;         
                }
              } else {  // if its running now
                if (stripArray.remain() == stripArray.Empty) {  // buffer underrun - have to stop playback
                  isRunning = false;
                  canDisplay = false;
                } else {  // we can keep running
                  //sRunning = true; //useless
                  canDisplay = true;     
                }
                
              }
        
              
              if (canDisplay) {
                if (MYDEBUGSERIAL) Serial.println("COPY\r\n");
                if (MYDEBUGSERIAL) delay(100);
                
                sa = stripArray.pop();
                memcpy(getPixelPointer(),sa.ch,900);
              }   
              if (MYDEBUGSERIAL) Serial.print(".");   
              next_update += 1000/currentFps;
              delay(0);
            }
            if (MYDEBUGSERIAL) Serial.println();
            // show the last!
            if (canDisplay) {
              Led_Show();
              if (MYDEBUGSERIAL) Serial.println("SHOW\r\n");
              if (MYDEBUGSERIAL) delay(100);
            }
          }
      }
  } else  if (currentProgram == PROG_TEST) {
    int bytes=ParseUDP(currentProgram);
    if (!ModeChanged) {
      if (bytes) {
        Led_Show();
      }
    }
  }
  

  
  



}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') nextProg=true;
  }
}

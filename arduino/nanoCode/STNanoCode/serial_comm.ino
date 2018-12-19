#include "serial_comm.h"
#include "common.h"

SoftwareSerial mySerial(2, 7); // RX, TX

bool enable_debug=false;

char StartOfFrame[]="SOF";
char EndOfFrame[]="EOF";
char myRXBuffer[RX_BUFF_SIZE];
int rxPos=0;
bool hasPixelData=false;

void DebugWrite(const char * pString) {
  if (enable_debug)
    Serial.write(pString);
}

void InitSerials(void) {
  mySerial.begin(115200);
  Serial.begin(115200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  while (!mySerial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  DebugWrite("Serial started\r\n");
}

void ProcessRxFrame(void) {
    rxPos++;
    DebugWrite("\r\nPacket received!\r\n");
    toggle_led();
    //Serial.write(myRXBuffer,rxPos);
    //Serial.write("\r\n");

    hasPixelData=true;
    
}

void resetRXBuffer(void) {
  memset(myRXBuffer,0,RX_BUFF_SIZE);
  rxPos=0;
  DebugWrite("DELETED\r\n");
}

bool checkRxFrame() {
  if (myRXBuffer[rxPos] == EndOfFrame[2]) {
    // possible end of frame?
    if (
        myRXBuffer[rxPos] == EndOfFrame[2] &&
        myRXBuffer[rxPos-1] == EndOfFrame[1] &&
        myRXBuffer[rxPos-2] == EndOfFrame[0] ) {
          // good!
          // check beginning of frame
          if (
            myRXBuffer[0] == StartOfFrame[0] &&
            myRXBuffer[1] == StartOfFrame[1] &&
            myRXBuffer[2] == StartOfFrame[2] ) {
              //start is also good!
              return true;
            } else {
                // No start ->  reset
                resetRXBuffer();
                return false;
            }
      }
  } 

  rxPos++;
  return false;
}

bool receiveSerials(void) {
  // receive if possible
  
  while (mySerial.available()) {
      
      if (rxPos<RX_BUFF_SIZE) {
          myRXBuffer[rxPos]=mySerial.read();
         
          if (checkRxFrame())
            ProcessRxFrame();

      } else {
        // overflow? clear buffer!
        resetRXBuffer();
        break;
      }
  }

  while (Serial.available()) {
    
    if (rxPos<RX_BUFF_SIZE) {
        myRXBuffer[rxPos]=Serial.read();
        
        if (checkRxFrame())
          ProcessRxFrame();

    } else {
      // overflow? clear buffer!
      resetRXBuffer();
      break;
    }
  }
  
}

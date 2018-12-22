/* 
 *  
 *  WIFI STUFF
 *  
 */

#include "xmas.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

CircularBuffer <stripArrayStruct,16> stripArray;
stripArrayStruct udpStripArray;
stripArrayStruct udpStripArray2;

const char *ssid = "UPC1837310";
const char *password = "";
const char* hostn = "xmastree";

MDNSResponder mdns;
ESP8266WebServer server ( 80 );

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
WiFiUDP udp2;
unsigned int localPort = 8081;      // local port to listen for UDP packets
const int PACKET_SIZE = 1000; 
byte packetBuffer[ PACKET_SIZE]; //buffer to hold incoming and outgoing packets

byte txSerialPacket[PACKET_SIZE]; //buff to send serial


void Wifi_HandleClients() {
   server.handleClient();
}

//  <meta http-equiv='refresh' content='5'/> // r

void handleRoot() {
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( temp, 400,
"<html>\
  <head>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <h2>Futo prg: %d</h2>\
    <h2><a href='/next'>Kovetkezo progi</a></h2>\
  </body>\
</html>",

    hr, min % 60, sec % 60, getcPrg()
  );
  server.send ( 200, "text/html", temp );
}

void handleNext() {
  server.send( 200,"text/html", "<meta http-equiv='refresh' content='0; url=/' />");
  Serial.println("Next prg...");
  nextProg=true;
}

void handleNotFound() {
 // digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  //digitalWrite ( led, 0 );
}

enum { 
       W_INIT=0,
       W_WAIT,
       W_READY,
       W_FAIL   
      };
       
void ShowWifiStatus(int s) {
  uint8_t r=0;
  uint8_t g=0;
  uint8_t b=0;
  switch(s) {
    case W_INIT:  // feh�r
      r=g=b=127;
      break;
    case W_WAIT: // k�k
      r=g=0; b=127;
      break;
    case W_READY: // z�d
      r=b=0; g=127;
      break;
    case W_FAIL: // peros
      r=127; g=b=0;
      break;
    default: // lila
      r=b=127; g=0;
      break;
  }
  for(int i=0;i<NUMPIXELS;i++){
    if ((i<5)||(i>(NUMPIXELS-5))) pixels.setPixelColor(i,r,g,b); else pixels.setPixelColor(i,0,0,0);
  }  
  pixels.show();
}
  
void Wifi_Setup() {
  int timeout=0;
  ShowWifiStatus(W_INIT);
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );
  ShowWifiStatus(W_WAIT);
  while (( WiFi.status() != WL_CONNECTED ) && (timeout < 20)) {
    delay ( 500 );
    Serial.print ( "." );
    timeout++;
  }
  ShowWifiStatus(( WiFi.status() == WL_CONNECTED )?W_READY:W_FAIL);
  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if (mdns.begin(hostn, WiFi.localIP())) {
    Serial.println("MDNS responder started");
    Serial.print("You can now connect to http://");
    Serial.print(hostn);
    Serial.println(".local");
  }
  
  server.on ( "/", handleRoot );
//  server.on ( "/test.svg", drawGraph );
  server.on ("/next", handleNext );
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );

  //nlga stuff
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  
  delay(1000);
}



int ParseUDP(currentProgramType _type) {

  int cb = udp.parsePacket();
  if (cb) {
    if (MYDEBUGSERIAL) Serial.println("packet!");
    lastUDPPacketReceived = millis();

    if (cb > PACKET_SIZE) // upper limit!
      cb = PACKET_SIZE;
    udp.read(packetBuffer, cb); // read the packet into the buffer
    //memcpy(pixels.getPixels(),&packetBuffer[11],900); // displaying?
    if (cb<13)  // smaller than a pure header
      return 0;

    //check whether a mode-change command?
    if ( packetBuffer[9] == PROG_AUTO ) {
      ChangeProgram(PROG_AUTO);
      Serial.println("AUTO-MODE");    delay(100);
      //ModeChanged = true;
      return 1;
    } else if ( packetBuffer[9] == PROG_MANUAL ) {
      ChangeProgram(PROG_MANUAL);
      Serial.println("MANUAL-MODE");    delay(100);
      //ModeChanged = true;
      return 1;
    } else if ( packetBuffer[9] == PROG_TEST ) {
      ChangeProgram(PROG_TEST);
      Serial.println("TEST-MODE");    delay(100);
      //ModeChanged = true;
      return 1;
    } else if ( packetBuffer[9] == CMD_RAW24 ) {

        if (_type == PROG_MANUAL) { // ADD TO FIFO
          if (stripArray.remain() != stripArray.Full) {
            if (MYDEBUGSERIAL) Serial.println("added!");
            memcpy(udpStripArray.ch,&packetBuffer[11],900);
            stripArray.push(udpStripArray);
          }
        } else if (_type == PROG_TEST) { // DISPLAY
            memcpy(pixels.getPixels(),&packetBuffer[11],900); // displaying?
        }
    } else if ( packetBuffer[9] == CMD_RAW24_2 ) {
        if (_type == PROG_TEST) { // DISPLAY
            //memcpy(txSerialPacket,&packetBuffer[0],cb); // pass-through

            
            int pixels = cb - 14; // all pre post header included

            //copy first part
            memcpy(txSerialPacket,&packetBuffer[0],11); // 11b

            byte * pSrcPix=&packetBuffer[11];
                        
            for(int i=0; i<pixels ; i++) {
               
                byte red = (*pSrcPix)&0xe0;
                byte green = ((*(pSrcPix+1))>>3)&0x1c;
                byte blue = (*(pSrcPix+2))>>5;
                
                txSerialPacket[11+i]= red | green | blue;
                pSrcPix+=3;
            }
            memcpy(&txSerialPacket[11+pixels],"EOF",3); // 11b
            
            
            /*
            Serial.write("RGB24_cmd_received:");
            Serial.print(cb);
            Serial.write("\r\n");
            */
            Serial1.write(txSerialPacket,cb); 
            Serial1.flush();
            delay(5);   //5 ms delay
        }
    } else if ( packetBuffer[9] == CMD_RAW8 ) {
            memcpy(txSerialPacket,&packetBuffer[0],cb); // pass-through
            Serial1.write(txSerialPacket,cb); 
            Serial1.flush();
            delay(5);   //5 ms delay
    } else if ( packetBuffer[9] == CMD_SET_FPS ) {
      currentFps = (unsigned int) packetBuffer[11];
      if ( currentFps > MAX_FPS)
        currentFps=MAX_FPS;
        Serial.printf("New fps= %d\n",currentFps);    delay(100);
    } else if ( packetBuffer[9] == CMD_PGM_START ) {
        int newPgm=(unsigned int) packetBuffer[11];
        ChangeProgram(PROG_AUTO);
        startPrg(newPgm);
    }
  }
  
  return cb;
}

byte packetBuffer2[ 10];
IPAddress bcip(255, 255, 255, 255);
// send an NTP request to the time server at the given address
unsigned long sendUDPpacket(void)
{
  
  
  //IPAddress bcip(192, 168, 0, 1);
    // set all bytes in the buffer to 0
  memset(packetBuffer2, 0, PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  noInterrupts();

  udp2.beginPacket(bcip, 8082); //NTP requests are to port 123
  udp2.write(packetBuffer2, 4);
  udp2.endPacket();

  interrupts();

  //udp2.stop();
  
  
}

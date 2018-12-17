#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H


#include <stdio.h>
#include <SoftwareSerial.h>

#define RX_BUFF_SIZE  1024

bool receiveSerials(void);
void InitSerials(void);


extern char myRXBuffer[RX_BUFF_SIZE];
extern int rxPos;
extern bool hasPixelData;

/*
 * communication (always MSB first)
 * 
 * header=(total 14b)
 * 
 * SOF 3b
 * data length 2b
 * crc 2b
 * sequence 2b
 * msg type 1b
 * msg reserved 1b
 * data bytes nb
 * EOF 3b
 * 
 */


 #endif

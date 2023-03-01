#ifndef __ARDUCAM_SLOT_H
#define __ARDUCAM_SLOT_H
#include <Arduino.h>

#define SerialWriteBuff(buf, len)   Serial.write(buf, len)
#define SerialBegin(baudRate)       Serial.begin(baudRate)
#define SerialWrite(ch)             Serial.write(ch)
#define SerialPrintf(str)           Serial.print(str)
#define SerialAvailable()           Serial.available()
#define SerialRead()                Serial.read()
#endif
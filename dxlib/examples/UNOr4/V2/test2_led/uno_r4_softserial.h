#pragma once

#if defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOWIFIR4)

#include  <stdint.h>
#include  <SoftwareSerial.h>
#include  <dx2lib.h>

#define MY_RX_PIN 8
#define MY_TX_PIN 9

SoftwareSerial *mySerial = NULL;

//SoftwareSerial mySerial(MY_RX_PIN, MY_TX_PIN);  // RX, TX

const DX2LIB::TDXHost_ConfParam param = {
  [](uint32_t baud) {
    if (mySerial != NULL) {
      mySerial->end();
      delete mySerial;
    }
    mySerial = new SoftwareSerial (MY_RX_PIN, MY_TX_PIN);
    mySerial->begin(baud);
    return baud;
  },
  [] {
    if (mySerial != NULL) {
      mySerial->end();
      delete mySerial; 
      mySerial = NULL;
    }
  },
  [](uint32_t baud) {
    if (mySerial != NULL) {
      mySerial->end();
      delete mySerial; 
    }
    mySerial = new SoftwareSerial (MY_RX_PIN, MY_TX_PIN);
    mySerial->begin(baud);
    return baud;
  },
  [](uint32_t timeout) {
    if (mySerial != NULL) {
      mySerial->setTimeout(timeout);
    }
    return timeout;
  },
  [] {
    while (mySerial->available()) mySerial->read();
  },
  [](uint8_t c) {
    mySerial->write(c);
  },
  [](const uint8_t *buf, int len) {
    mySerial->write(buf, len);
  },
  [](uint8_t *buf, int len) {
    if (len == 0) return 0;
    return (int)mySerial->readBytes(buf, len);
  },
  [] {
    mySerial->flush();
  }
};

#endif

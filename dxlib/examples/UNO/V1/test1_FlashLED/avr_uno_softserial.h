#pragma once

// Collection of routines for initialization, sending and receiving using
// SOFTWARESERIAL with GPIO.

#if HARDWARE_TYPE == AVR_UNO

#include  <stdint.h>
#include  <SoftwareSerial.h>
#include  <dxlib.h>

#define MY_RX_PIN 8
#define MY_TX_PIN 9

SoftwareSerial mySerial(MY_RX_PIN, MY_TX_PIN);  // RX, TX

const DXLIB::TDXHost_ConfParam param = {
  [](uint32_t baud) {
    mySerial.begin(baud);
    return baud;
  },
  [] {
    mySerial.end();
  },
  [](uint32_t baud) {
    mySerial.begin(baud);
    return baud;
  },
  [](uint32_t timeout) {
    mySerial.setTimeout(timeout);
    return timeout;
  },
  [] {
    while (mySerial.available()) mySerial.read();
  },
  [](uint8_t c) {
    mySerial.write(c);
  },
  [](const uint8_t *buf, int len) {
    mySerial.write(buf, len);
  },
  [](uint8_t *buf, int len) {
    if (len == 0) return 0;
    return (int)mySerial.readBytes(buf, len);
  },
  [] {
    mySerial.flush();
  }
};

#endif

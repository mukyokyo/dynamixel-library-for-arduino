#pragma once

// Collection of routines for initialization, sending and receiving using
// HARDWARESERIAL.

#if HARDWARE_TYPE == AVR_UNO

#include  <stdint.h>
#include  <dx2lib.h>

const DX2LIB::TDXHost_ConfParam param = {
  [](uint32_t baud) {
    Serial.begin(baud);
    return baud;
  },
  [] {
    Serial.end();
  },
  [](uint32_t baud) {
    Serial.begin(baud);
    return baud;
  },
  [](uint32_t timeout) {
    Serial.setTimeout(timeout);
    return timeout;
  },
  [] {
    while (Serial.available()) Serial.read();
  },
  [](uint8_t c) {
    Serial.write(c);
  },
  [](const uint8_t *buf, int len) {
    Serial.write(buf, len);
  },
  [](uint8_t *buf, int len) {
    if (len == 0) return 0;
    return (int)Serial.readBytes(buf, len);
  },
  [] {
    Serial.flush();
  }
};

#endif
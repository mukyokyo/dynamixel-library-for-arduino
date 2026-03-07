#pragma once

#if defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOWIFIR4)

#include  <stdint.h>
#include  <dx2lib.h>

const DX2LIB::TDXHost_ConfParam param = {
  [](uint32_t baud) {
    Serial1.begin(baud);
    return baud;
  },
  [] {
    Serial1.end();
  },
  [](uint32_t baud) {
    Serial1.begin(baud);
    return baud;
  },
  [](uint32_t timeout) {
    Serial1.setTimeout(timeout);
    return timeout;
  },
  [] {
    while (Serial1.available()) Serial1.read();
  },
  [](uint8_t c) {
    Serial1.write(c);
  },
  [](const uint8_t *buf, int len) {
    Serial1.write(buf, len);
  },
  [](uint8_t *buf, int len) {
    if (len == 0) return 0;
    return (int)Serial1.readBytes(buf, len);
  },
  [] {
    Serial1.flush();
  }
};

#endif

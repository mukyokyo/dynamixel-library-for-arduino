## Overview

Library for Controlling DXL from the Arduino Series.

## Requirement

- Arduino IDE
- Arduino UNO, UNO R4, UNO Q, etc

DXL has a half-duplex interface, but Arduino does not. This primarily assumes hardware devices such as [DXSHIELD](https://www.besttechnology.co.jp/modules/knowledge/?BTE074B%20DXSHIELD), [PicoSHIELD](https://www.besttechnology.co.jp/modules/knowledge/?BTE110%20PicoSHIELD), [XiaoSHIELD](https://www.besttechnology.co.jp/modules/knowledge/?XiaoSHIELD) and [TTL2DXIF](https://www.besttechnology.co.jp/modules/knowledge/?BTE094B%20TTL2DXIF)
 that support half-duplex operation.

Of course, hardware implementing logic to switch buses itself can be used, but since it depends on the platform, we do not provide it as sample code.

## Usage

Please look up how to install it on the Arduino IDE yourself.

Using UNO R4 and the mainstream Protocol V2 as examples.<br>
Create the callback for the serial communication portion dependent on Arduino, and initialization is complete when specified during the instantiation of the library class.

```c
// Include the library header
#include <dx2lib.h>

// Prepare callback routines for basic transmission and reception.
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

// Instantiate a class from the library
DX2LIB dx2 ((DX2LIB::TDXHost_ConfParam *)&param);

void setup() {
  // set baudrate & timeout[ms]
  dx2.begin (BAUDRATE, 50);
}

void loop() {
}
```

Once initialization is complete, the API can be used according to the instructions. Most APIs return success or failure, and all value exchanges are passed via arguments.<br>
DXL provides a control table for exchanging IDs, addresses, and data. APIs are categorized based on how they access this control table.<br>
The main APIs are summarized as follows.

```c
bool ReadBlockData(uint8_t id, uint16_t addr, uint8_t *data, uint16_t len, uint8_t *err);

bool WriteBlockData(uint8_t id, uint16_t addr, const uint8_t *data, uint16_t len, uint8_t *err);

bool WriteSyncByteData(const uint8_t *pid, uint16_t addr, const uint8_t *pdata, int num);

bool ReadSyncData(const TSyncReadParam *param, uint32_t *num, uint8_t *dat, uint8_t *err);

bool WriteBulkData(uint8_t *dat, uint32_t size);

bool ReadBulkData(const TBulkReadParam *param, uint8_t *num, uint8_t *dat, uint8_t *err);

bool Ping(uint8_t id, uint8_t *err);
bool Ping2(uint8_t *num, TAlarmStatus *AlarmStatus, uint8_t *err);
bool Reset(uint8_t id, uint8_t *err);
bool Reboot(uint8_t id, uint8_t *err);
```

## Licence

[MIT](https://github.com/mukyokyo/dynamixel-library-for-arduino/blob/main/LICENSE)

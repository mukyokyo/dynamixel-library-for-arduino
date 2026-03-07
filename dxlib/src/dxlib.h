/*
  dxlib.h
  Dynamixel Protocol V.1 Library for DXSHIELD

  Copyright (c) 2016-2026 BestTechnology CO.,LTD.
*/

#pragma once

#include <Arduino.h>

class DXLIB {
public:
  typedef struct {
    uint32_t (*init)(uint32_t);
    void (*deinit)(void);
    uint32_t (*setbaudrate)(uint32_t);
    uint32_t (*settimeout)(uint32_t);
    void (*rx_purge)(void);
    void (*_putc)(uint8_t);
    void (*_puts)(const uint8_t *, int);
    int (*_gets)(uint8_t *, int);
    void (*flush)(void);
  } TDXHost_ConfParam, *PDXHost_ConfParam;

  void (*SemLock)(void) = nullptr;
  void (*SemFree)(void) = nullptr;

  DXLIB(const PDXHost_ConfParam param);
  ~DXLIB();
  void begin (long baud, uint32_t timeout);
  void end(void);

  bool TxPacket (uint8_t id, uint8_t inst, uint8_t *param, uint16_t len);
  bool RxPacket (uint8_t *rdata, uint16_t rdatasize, uint8_t *rid, uint16_t *rlen, uint8_t *err = nullptr);

  bool ReadBlockData (uint8_t id, uint16_t addr, uint8_t *data, uint16_t len, uint8_t *err = nullptr);
  inline bool ReadByteData (uint8_t id, uint16_t addr, uint8_t *data, uint8_t *err = nullptr) {
    return ReadBlockData (id, addr, (uint8_t *)data, 1, err);
  }
  inline bool ReadByteData (uint8_t id, uint16_t addr, int8_t data, uint8_t *err = nullptr) {
    return ReadBlockData (id, addr, (uint8_t *)data, 1, err);
  }
  inline bool ReadWordData (uint8_t id, uint16_t addr, uint16_t *data, uint8_t *err = nullptr) {
    return ReadBlockData (id, addr, (uint8_t *)data, 2, err);
  }
  inline bool ReadWordData (uint8_t id, uint16_t addr, int16_t data, uint8_t *err = nullptr) {
    return ReadBlockData (id, addr, (uint8_t *)data, 2, err);
  }
  inline bool ReadLongData (uint8_t id, uint16_t addr, uint32_t *data, uint8_t *err = nullptr) {
    return ReadBlockData (id, addr, (uint8_t *)data, 4, err);
  }
  inline bool ReadLongData (uint8_t id, uint16_t addr, int32_t data, uint8_t *err = nullptr) {
    return ReadBlockData (id, addr, (uint8_t *)data, 4, err);
  }

  bool WriteBlockData (uint8_t id, uint16_t addr, const uint8_t *dat, uint16_t len, uint8_t *err = nullptr);
  inline bool WriteByteData (uint8_t id, uint16_t addr, uint8_t data, uint8_t *err = nullptr) {
    return WriteBlockData (id, addr, (uint8_t *)&data, 1, err);
  }
  inline bool WriteWordData (uint8_t id, uint16_t addr, uint16_t data, uint8_t *err = nullptr) {
    return WriteBlockData (id, addr, (uint8_t *)&data, 2, err);
  }
  inline bool WriteLongData (uint8_t id, uint16_t addr, uint32_t data, uint8_t *err = nullptr) {
    return WriteBlockData (id, addr, (uint8_t *)&data, 4, err);
  }

  bool WriteSyncByteData (const uint8_t *pid, uint16_t addr, const uint8_t *pdata, int num);
  inline bool WriteSyncByteData (const uint8_t *pid, uint16_t addr, const int8_t *pdata, int num) {
    return WriteSyncByteData (pid, addr, (const uint8_t *)pdata, num);
  }
  bool WriteSyncWordData (const uint8_t *pid, uint16_t addr, const uint16_t *pdata, int num);
  inline bool WriteSyncWordData (const uint8_t *pid, uint16_t addr, const int16_t *pdata, int num) {
    return WriteSyncWordData (pid, addr, (const uint16_t *)pdata, num);
  }
  bool WriteSyncLongData (const uint8_t *pid, uint16_t addr, const uint32_t *pdata, int num);
  inline bool WriteSyncLongData (const uint8_t *pid, uint16_t addr, const int32_t *pdata, int num) {
    return WriteSyncLongData (pid, addr, (const uint32_t *)pdata, num);
  }

  bool Ping (uint8_t id, uint8_t *err = nullptr);
  bool Reset (uint8_t id, uint8_t *err = nullptr);
private:
  PDXHost_ConfParam dxif;
  uint32_t baudrate;
  uint32_t offsettimeout;
  bool active (void);
};
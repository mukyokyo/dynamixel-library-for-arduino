/*
  dxlib.cpp
  Dynamixel Protocol V.1 Library for DXSHIELD

  Copyright (c) 2016-2026 BestTechnology CO.,LTD.
*/

#include "dxlib.h"

#define LOCK \
  { \
    if (SemLock != nullptr) SemLock(); \
  }
#define FREE \
  { \
    if (SemFree != nullptr) SemFree(); \
  }

DXLIB::DXLIB(const PDXHost_ConfParam param)
  : dxif(nullptr) {
  dxif = param;
}

void DXLIB::begin (long baud, uint32_t timeout) {
  baudrate = baud;
  dxif->init(baud);
  offsettimeout = dxif->settimeout(timeout);
}

DXLIB::~DXLIB() {
  if (active()) dxif->deinit();
}

bool DXLIB::active (void) {
  return (dxif != nullptr);
}


bool DXLIB::TxPacket (uint8_t id, uint8_t inst, uint8_t *param, uint16_t len) {
  if (!active()) return false;
  if (id > 254) return false;
  uint8_t sum = 0, buf[5] = {0xff, 0xff, id, (uint8_t) (len + 2), inst};
  dxif->rx_purge();
  for (int i = 2; i < 5; i++) sum += buf[i];
  dxif->_puts (buf, 5);
  if (len > 0) {
    for (uint16_t i = 0; i < len; i++) sum += param[i];
    dxif->_puts (param, len);
  }
  sum = ~sum;
  dxif->_puts (&sum, 1);
  dxif->flush();
  return true;
}

bool DXLIB::RxPacket (uint8_t *rdata, uint16_t rdatasize, uint8_t *rid, uint16_t *rlen, uint8_t *err) {
  if (!active()) return false;
  if (rdata == NULL) return false;
  if (rlen == NULL) return false;
  uint8_t buf[5];
  if (dxif->_gets (buf, 5) == 5) {
    if ((* (uint16_t *)&buf[0] == 0xffffUL) && (buf[2] <= 0xfd) && (buf[3] >= 2)) {
      if (rid != NULL) *rid = buf[2];
      if (err != NULL) *err = buf[4];
      uint8_t len = buf[3] - 2;
      uint8_t sum = buf[2] + buf[3] + buf[4];
      if (len <= rdatasize) {
        if (dxif->_gets (rdata, len) == len) {
          *rlen = len;
          for (int i = 0; i < len; i++) sum += rdata[i];
          if (dxif->_gets (buf, 1) == 1) return (uint8_t (~sum) == buf[0]);
        }
      }
    }
  }
  *rlen = 0;
  return false;
}

// read block data
bool DXLIB::ReadBlockData (uint8_t id, uint16_t addr, uint8_t *data, uint16_t len, uint8_t *err) {
  bool result = false;
  LOCK;
  uint8_t buf[2] = { (uint8_t)addr, (uint8_t)len};
  uint8_t rid, rerr = 0;
  uint16_t l;
  if (TxPacket (id, 0x02, buf, 2))
    if (RxPacket (data, len, &rid, &l, &rerr))
      result = ((l == len) && (rid == id) && ((rerr & 0x5a) == 0));
  if (err != NULL) *err = rerr;
  FREE;
  return result;
}

// write block data
bool DXLIB::WriteBlockData (uint8_t id, uint16_t addr, const uint8_t *dat, uint16_t len, uint8_t *err) {
  bool result = false;
  LOCK;
  uint8_t buf[6] = {0xff, 0xff, id, uint8_t (len + 3), 0x03, (uint8_t)addr};
  uint8_t sum = 0;
  uint8_t rid, rerr = 0;
  uint16_t l;
  if (id <= 0xFE) {
    dxif->rx_purge();
    for (int i = 2; i <= 5; i++) sum += buf[i];
    for (int i = 0; i < (int)len; i++) sum += dat[i];
    sum = ~sum;
    dxif->_puts (buf, 6);
    dxif->_puts (dat, len);
    dxif->_puts (&sum, 1);

    if (RxPacket (buf, sizeof (buf), &rid, &l, &rerr))
      result = (rid == id) && (l == 0) && ((rerr & 0x5a) == 0);
  }
  if (err != NULL) *err = rerr;
  FREE;
  return result;
}

// sync write byte
bool DXLIB::WriteSyncByteData (const uint8_t *pid, uint16_t addr, const uint8_t *pdata, int num) {
  if (!active()) return false;
  LOCK;
  uint8_t buf[7] = {0xff, 0xff, 0xfe, (uint8_t) ((num * 2 + 4) & 0xff), 0x83, (uint8_t)addr, 1};
  uint8_t sum = 0;
  for (int i = 2; i < 7; i++) sum += buf[i];
  dxif->rx_purge();
  dxif->_puts (buf, 7);
  for (int i = 0; i < num; i++) {
    sum += pid[i];
    dxif->_putc (pid[i]);
    sum += pdata[i];
    dxif->_putc (pdata[i]);
  }
  dxif->_putc (~sum);
  dxif->flush();
  FREE;
  return true;
}

// sync write word
bool DXLIB::WriteSyncWordData (const uint8_t *pid, uint16_t addr, const uint16_t *pdata, int num) {
  if (!active()) return false;
  LOCK;
  uint8_t buf[7] = {0xff, 0xff, 0xfe, (uint8_t) ((num * 3 + 4) & 0xff), 0x83, (uint8_t)addr, 2};
  uint8_t sum = 0;
  for (int i = 2; i < 7; i++) sum += buf[i];
  dxif->rx_purge();
  dxif->_puts (buf, 7);
  for (int i = 0; i < num; i++) {
    sum += pid[i];
    dxif->_putc (pid[i]);
    sum += (pdata[i] & 0xff);
    dxif->_putc ((pdata[i] & 0xff));
    sum += ((pdata[i] >> 8) & 0xff);
    dxif->_putc ((pdata[i] >> 8) & 0xff);
  }
  dxif->_putc (~sum);
  dxif->flush();
  FREE;
  return true;
}

// sync write long
bool DXLIB::WriteSyncLongData (const uint8_t *pid, uint16_t addr, const uint32_t *pdata, int num) {
  if (!active()) return false;
  LOCK;
  uint8_t buf[7] = {0xff, 0xff, 0xfe, (uint8_t) ((num * 5 + 4) & 0xff), 0x83, (uint8_t)addr, 4};
  uint8_t sum = 0;
  for (int i = 2; i < 7; i++) sum += buf[i];
  dxif->rx_purge();
  dxif->_puts (buf, 7);
  for (int i = 0; i < num; i++) {
    sum += pid[i];
    dxif->_putc (pid[i]);
    sum += (pdata[i] & 0xff);
    dxif->_putc ((pdata[i] & 0xff));
    sum += ((pdata[i] >> 8) & 0xff);
    dxif->_putc ((pdata[i] >> 8) & 0xff);
    sum += ((pdata[i] >> 16) & 0xff);
    dxif->_putc ((pdata[i] >> 16) & 0xff);
    sum += ((pdata[i] >> 24) & 0xff);
    dxif->_putc ((pdata[i] >> 24) & 0xff);
  }
  dxif->_putc (~sum);
  dxif->flush();
  FREE;
  return true;
}

// ping
bool DXLIB::Ping (uint8_t id, uint8_t *err) {
  if (id > 253) return false;
  LOCK;
  uint8_t buf[5];
  bool result = false;
  if (TxPacket (id, 0x01, NULL, 0)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x5a) == 0);
    if (err != NULL) *err = rerr;
  }
  FREE;
  return result;
}

// factory reset
bool DXLIB::Reset (uint8_t id, uint8_t *err) {
  if (id >= 254) return false;
  LOCK;
  uint8_t buf[5];
  bool result = false;
  if (TxPacket (id, 0x06, NULL, 0)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x5a) == 0);
    if (err != NULL) *err = rerr;
  }
  FREE;
  return result;
}

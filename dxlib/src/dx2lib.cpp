/*
  dx2lib.cpp
  Dynamixel Protocol V.2 Library

  Copyright (c) 2016-2026 BestTechnology CO.,LTD.
*/

#include <Arduino.h>
#include <string.h>
#include "dx2lib.h"
#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#ifndef PROGMEM
#define PROGMEM
#endif
#endif

#define LOCK \
  { \
    if (SemLock != nullptr) SemLock(); \
  }
#define FREE \
  { \
    if (SemFree != nullptr) SemFree(); \
  }

DX2LIB::DX2LIB(const PDXHost_ConfParam param)
  : dxif(nullptr) {
  dxif = param;
}

DX2LIB::~DX2LIB() {
  if (active()) dxif->deinit();
  dxif = nullptr;
}

void DX2LIB::begin(uint32_t baud, uint32_t timeout) {
  baudrate = baud;
  dxif->init(baud);
  offsettimeout = dxif->settimeout(timeout);
}

void DX2LIB::end() {
  if (active()) dxif->deinit();
}

bool DX2LIB::active(void) {
  return (dxif != nullptr);
}

uint16_t DX2LIB::crc16(uint16_t *crc, const uint8_t *dat, uint16_t size) {
  const uint16_t crc16_lookuptable[256] PROGMEM = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
  };
  for (uint16_t i = 0; i < size; i++) *crc = (*crc << 8) ^ crc16_lookuptable[(((*crc >> 8) ^ dat[i]) & 0xff)];
  return *crc;
}

uint16_t DX2LIB::addsuffix(const uint8_t *dat, uint16_t len, bool tx, uint16_t *crc) {
  uint16_t exlen = 0;
  uint8_t match = 0;
  for (uint16_t i = 0; i < len; i++) {
    if (tx) {
      crc16(crc, dat, 1);
      dxif->_putc(*dat);
    }
    switch (*dat) {
      case 0xff:
        match++;
        break;
      case 0xfd:
        if (match == 2) {
          exlen++;
          if (tx) {
            crc16(crc, dat, 1);
            dxif->_putc(0xfd);
          }
        }
        match = 0;
        break;
      default:
        match = 0;
        break;
    }
    dat++;
  }
  return exlen;
}

// calc timeout
double DX2LIB::CalcTimeout(uint32_t baud, uint32_t num) {
  return (10000.0 * (double)num) / (double)baud;
}

bool DX2LIB::TxPacket(uint8_t id, uint8_t inst, uint8_t *param, uint16_t len) {
  if (!active()) return false;
  if ((id == 255) || (id == 253)) return false;
#ifndef __AVR__
  if (len + 10 > 512) return false;
  int patternmatch = 0, exlen = 0;
  static uint8_t buf[512];
  uint16_t crc = 0;

  *(uint32_t *)(void *)&buf[0] = 0x00fdffffU;
  buf[4] = id;
  buf[7] = inst;

  for (int i = 0; i < len; i++) {
    switch (buf[8 + i + exlen] = param[i]) {
      case 0xff:
        patternmatch++;
        break;
      case 0xfd:
        if (patternmatch == 2) {
          exlen++;
          buf[8 + i + exlen] = 0xfd;
        }
        patternmatch = 0;
        break;
      default:
        patternmatch = 0;
        break;
    }
  }
  // データ長 (INST+PARAM数+Suffix数+CRC)
  //  *(uint16_t *)(void *)&buf[5] = len + exlen + 3;
  buf[5] = (len + exlen + 3) & 0xff;
  buf[6] = ((len + exlen + 3) >> 8) & 0xff;

  // CRC
  crc16(&crc, buf, 8 + len + exlen);
  //  * (uint16_t *) (void *)&buf[8 + len + exlen] = crc;
  buf[8 + len + exlen] = crc & 0xff;
  buf[9 + len + exlen] = (crc >> 8) & 0xff;
  len += exlen + 10;
  dxif->rx_purge();
  dxif->_puts(buf, len);
#else
  uint16_t crc = 0x0e28;
  uint8_t buf[8] = { 0xff, 0xff, 0xfd, 0, id, 0x00, 0x00, inst };
  *(uint16_t *)&buf[5] = len + 3 + addsuffix(param, len, false, nullptr);
  crc16(&crc, &buf[4], 4);
  dxif->rx_purge();
  dxif->_puts(buf, 8);
  if (len > 0) addsuffix(param, len, true, &crc);
  dxif->_puts((uint8_t *)&crc, 2);
#endif
  dxif->flush();
  return true;
}

static uint16_t squeeze(uint8_t *dest, uint8_t *src, uint16_t len) {
  uint32_t *p, l = 0;
  for (uint16_t i = 0; i < len; i++) {
    if (i <= (uint16_t)(len - 4)) {
      p = (uint32_t *)(void *)src;
      if (*p == 0xfdfdffffU) {
        *(uint32_t *)(void *)dest = 0xfdffffU;
        src += 4;
        dest += 3;
        l += 3;
        i += 3;
      } else {
        *dest++ = *src++;
        l++;
      }
    } else {
      *dest++ = *src++;
      l++;
    }
  }
  return l;
}

bool DX2LIB::RxPacket(uint8_t *rdata, uint16_t rdatasize, uint8_t *rid, uint16_t *rlen, uint8_t *err) {
  if (!active()) return false;
  if (rdata == nullptr) return false;
  if (rlen == nullptr) return false;
  uint8_t buf[9];
  *rlen = 0;
  if (dxif->_gets(buf, 9) == 9) {
    if ((*(uint32_t *)&buf[0] == 0x00fdffffUL) && (buf[7] == 0x55) && (*(uint16_t *)&buf[5] >= 4)) {
      if (rid != nullptr) *rid = buf[4];
      if (err != nullptr) *err = buf[8];
      int len = (int)(*(uint16_t *)&buf[5] - 4);
      uint16_t crc = 0;
      crc16(&crc, buf, 9);
      if (dxif->_gets(rdata, len) == len) {
        crc16(&crc, rdata, len);
        if (dxif->_gets(buf, 2) == 2) {
          if (crc == *(uint16_t *)&buf[0]) {
            if (len > (int)rdatasize) *rlen = squeeze(rdata, rdata, len);
            else *rlen = len;
            return true;
          }
        }
      }
    }
  }
  return false;
}

// read block data
bool DX2LIB::ReadBlockData(uint8_t id, uint16_t addr, uint8_t *data, uint16_t len, uint8_t *err) {
  if (id > 252) return false;
  LOCK;
  bool result = false;
  uint16_t buf[2] = { addr, len };
  uint8_t rid, rerr = 0;
  uint16_t l;
  if (TxPacket(id, 0x02, (uint8_t *)&buf, 4))
    if (RxPacket(data, len, &rid, &l, &rerr))
      result = ((len == l) && (rid == id) && ((rerr & 0x7f) == 0));
  if (err != nullptr) *err = rerr;
  FREE;
  return result;
}

// write block data
bool DX2LIB::WriteBlockData(uint8_t id, uint16_t addr, const uint8_t *data, uint16_t len, uint8_t *err) {
  if (!active()) return false;
  if ((id == 255) || (id == 253)) return false;
  LOCK;
  bool result = false;
  uint8_t buf[10] = { 0xff, 0xff, 0xfd, 0x00, id, /*(len + 5)*/ 0, 0, 0x03, (uint8_t)(addr & 0xff), (uint8_t)(addr >> 8) };
  uint16_t crc = 0x0e28;
  uint8_t rid, rerr = 0;
  uint16_t l;
  *(uint16_t *)&buf[5] = len + 5 + addsuffix(data, len, false, nullptr);
  crc16(&crc, &buf[4], 6);
  dxif->rx_purge();
  dxif->_puts(buf, 10);
  if (len > 0) addsuffix(data, len, true, &crc);
  dxif->_puts((uint8_t *)&crc, 2);
  if (id <= 252) {
    if (RxPacket(buf, sizeof(buf), &rid, &l, &rerr))
      result = ((rid == id) && (l == 0) && ((rerr & 0x7f) == 0));
    if (err != nullptr) *err = rerr;
  } else
    result = true;
  FREE;
  return result;
}

// write sync byte
bool DX2LIB::WriteSyncByteData(const uint8_t *pid, uint16_t addr, const uint8_t *pdata, int num) {
  if (!active()) return false;
  for (int i = 0; i < num; i++)
    if (pid[i] > 252) return false;
  LOCK;
  uint16_t crc = 0x2a20;
  uint8_t buf[12] = { 0xff, 0xff, 0xfd, 0x00, 0xfe, /*(num * 2 + 7)*/ 0, 0, 0x83, /*addr*/ 0, 0, 1, 0 };
  *(uint16_t *)&buf[5] = num * 2 + 7;
  *(uint16_t *)&buf[8] = addr;
  crc16(&crc, &buf[5], 7);
  dxif->rx_purge();
  dxif->_puts(buf, 12);
  for (int i = 0; i < num; i++) {
    buf[0] = pid[i];
    buf[1] = pdata[i];
    dxif->_puts(buf, 2);
    crc16(&crc, buf, 2);
  }
  *(uint16_t *)&buf[0] = crc;
  dxif->_puts(buf, 2);
  dxif->flush();
  FREE;
  return true;
}

// write sync word
bool DX2LIB::WriteSyncWordData(const uint8_t *pid, uint16_t addr, const uint16_t *pdata, int num) {
  if (!active()) return false;
  for (int i = 0; i < num; i++)
    if (pid[i] > 252) return false;
  LOCK;
  uint16_t crc = 0x2a20;
  uint8_t buf[12] = { 0xff, 0xff, 0xfd, 0x00, 0xfe, /*(num * 3 + 7)*/ 0, 0, 0x83, /*addr*/ 0, 0, 2, 0 };
  *(uint16_t *)&buf[5] = num * 3 + 7;
  *(uint16_t *)&buf[8] = addr;
  crc16(&crc, &buf[5], 7);
  dxif->rx_purge();
  dxif->_puts(buf, 12);
  for (int i = 0; i < num; i++) {
    buf[0] = pid[i];
    *(uint16_t *)&buf[1] = pdata[i];
    dxif->_puts(buf, 3);
    crc16(&crc, buf, 4);
  }
  *(uint16_t *)&buf[0] = crc;
  dxif->_puts(buf, 2);
  dxif->flush();
  FREE;
  return true;
}

// write sync long
bool DX2LIB::WriteSyncLongData(const uint8_t *pid, uint16_t addr, const uint32_t *pdata, int num) {
  if (!active()) return false;
  LOCK;
  bool result = true;
  uint16_t crc = 0x2a20;
  uint16_t suffix = 0;
  uint8_t buf[12] = { 0xff, 0xff, 0xfd, 0x00, 0xfe, /*(num * 5 + 7)*/ 0, 0, 0x83, /*addr*/ 0, 0, 4, 0 };
  for (int i = 0; i < num; i++) {
    if (pid[i] > 252) {
      result = false;
      break;
    }
    if ((pdata[i] & 0x00ffffffUL) == 0xfdffffUL) suffix++;
    else if ((pdata[i] & 0xffffff00UL) == 0xfdffff00UL) suffix++;
  }
  if (result) {
    *(uint16_t *)&buf[5] = num * 5 + 7 + suffix;
    *(uint16_t *)&buf[8] = addr;
    crc16(&crc, &buf[5], 7);
    dxif->rx_purge();
    dxif->_puts(buf, 12);
    for (int i = 0; i < num; i++) {
      buf[0] = pid[i];
      if ((pdata[i] & 0x00ffffffUL) == 0x00fdffffUL) {
        *(uint32_t *)&buf[1] = 0xfdfdffffUL;
        buf[5] = (pdata[i] >> 24);
        dxif->_puts(buf, 6);
        crc16(&crc, buf, 6);
      } else if ((pdata[i] & 0xffffff00UL) == 0xfdffff00UL) {
        *(uint32_t *)&buf[2] = 0xfdfdffffUL;
        buf[1] = pdata[i] & 0xff;
        dxif->_puts(buf, 6);
        crc16(&crc, buf, 6);
      } else {
        *(uint32_t *)&buf[1] = pdata[i];
        dxif->_puts(buf, 5);
        crc16(&crc, buf, 5);
      }
    }
    *(uint16_t *)&buf[0] = crc;
    dxif->_puts(buf, 2);
    dxif->flush();
  } else
    FREE;
  return result;
}

// read sync data
bool DX2LIB::ReadSyncData(const TSyncReadParam *param, uint32_t *num, uint8_t *dat, uint8_t *err) {
  LOCK;
  int rnum = 0, acknum = 0;
  uint16_t l;
  bool result = false;

  if (err != nullptr) *err = ERR_NON;
  if (num != nullptr) {
    rnum = *num;
    *num = 0;
  }

  if (dat == nullptr) {
    if (err != nullptr) *err = ERR_INVALID_PARAM;
  } else if (num == nullptr) {
    if (err != nullptr) *err = ERR_INVALID_PARAM;
  } else if (rnum < 1 || 253 < rnum) {
    if (err != nullptr) *err = ERR_INVALID_PARAM;
  } else if (param->length > 1000) {
    if (err != nullptr) *err = ERR_ILLEGAL_SIZE;
  } else {
    uint8_t rbuf[param->length + (param->length / 3) + 1];
    memset(dat, 0xff, rnum * (param->length + 2));

    uint32_t prevtimeout = offsettimeout;
    uint32_t timeout1 = offsettimeout + CalcTimeout(baudrate, rnum * (11 + param->length));
    uint32_t timeout = offsettimeout + CalcTimeout(baudrate, (10 + 4 + rnum) + (rnum * (11 + param->length)));
    uint32_t detecttime = millis() + timeout;

    if (TxPacket(BROADCASTING_ID, INST_SYNC_READ, (uint8_t *)param, rnum + 4)) {
      while ((detecttime >= millis()) && (rnum > 0)) {
        uint8_t rid, rerr;
        dxif->settimeout(timeout1);
        if (RxPacket(rbuf, sizeof(rbuf), &rid, &l, /* timeout1, */ &rerr)) {
          if ((rid < BROADCASTING_ID) && (l > 0)) {
            int ofs = (param->length + 2) * acknum;
            dat[ofs] = rid;
            if (l == param->length) {
              dat[ofs + 1] = rerr;
              memcpy(&dat[ofs + 2], rbuf, param->length);
            } else {
              rerr |= ERR_ILLEGAL_SIZE;
              if (err != NULL) *err |= rerr;
              dat[ofs + 1] = rerr >> 8;
            }
            acknum++;
            rnum--;
          }
        } else {
          if (err != NULL) *err = rerr;
          break;
        }
      }
    }
    dxif->settimeout(prevtimeout);
    *num = acknum;
    result = (rnum == 0);
  }
  FREE;
  return result;
}

// write bulk data
bool DX2LIB::WriteBulkData(uint8_t *dat, uint32_t size) {
  LOCK;
  bool result = TxPacket(BROADCASTING_ID, INST_BULK_WRITE, dat, size);
  FREE;
  return result;
}

bool DX2LIB::ReadBulkData(const TBulkReadParam *param, uint8_t *num, uint8_t *dat, uint8_t *err) {
  LOCK;
  int rnum = 0, acknum = 0, ofs = 0;
  bool result = false;
  PBulkReadResult rdat;

  if (err != nullptr) *err = ERR_NON;
  if (num != nullptr) {
    rnum = *num;
    *num = 0;
  }

  if (dat == nullptr) {
    if (err != nullptr) *err = ERR_INVALID_PARAM;
  } else if (num == nullptr) {
    if (err != nullptr) *err = ERR_INVALID_PARAM;
  } else if (rnum < 1 || 253 < rnum) {
    if (err != nullptr) *err = ERR_INVALID_PARAM;
  } else {
    for (int i = 0; i < rnum; i++) {
      if (param[i].id >= BROADCASTING_ID) {
        if (err != nullptr) *err = ERR_INVALID_ID;
        return result;
      }
      if (param[i].length > 1000 || param[i].length < 1) {
        if (err != nullptr) *err = ERR_ILLEGAL_SIZE;
        return result;
      }
    }

    int maxlen = 0, totallen = 0;
    for (int i = 0; i < rnum; i++) {
      if (param[i].length > maxlen) maxlen = param[i].length;
      totallen += (param[i].length + 5);
    }
    memset(dat, 0, totallen);

    uint32_t prevtimeout = offsettimeout;
    uint32_t timeout1 = offsettimeout + CalcTimeout(baudrate, rnum * (11 + maxlen));
    uint32_t timeout = offsettimeout + CalcTimeout(baudrate, (10 + 4 + rnum) + (rnum * (11 + maxlen)));
    uint32_t detecttime = millis() + timeout;

    uint8_t rbuf[maxlen + (maxlen / 3) + 20];

    if (TxPacket(BROADCASTING_ID, INST_BULK_READ, (uint8_t *)param, rnum * 5)) {
      while ((detecttime >= millis()) && (rnum > 0)) {
        uint8_t rid, rerr;
        uint16_t l;
        dxif->settimeout(timeout1);
        if (RxPacket(rbuf, maxlen + 20, &rid, &l, /*timeout1, */ &rerr)) {
          if ((rbuf[4] < BROADCASTING_ID) && (l > 1)) {
            rdat = (PBulkReadResult)&dat[ofs];
            rdat->size = l + 4;
            rdat->id = rid;
            rdat->err = rerr;
            memcpy(&rdat->dat, rbuf, l);
            acknum++;
            rnum--;
            ofs += l + 4;
          }
        } else if (err != nullptr) *err |= rerr;
      }
    }
    dxif->settimeout(prevtimeout);
    *num = acknum;
    result = (rnum == 0);
  }
  FREE;
  return result;
}

// ping
bool DX2LIB::Ping(uint8_t id, uint8_t *err) {
  if (id > 252) return false;
  LOCK;
  uint8_t buf[10];
  bool result = false;
  if (TxPacket(id, 0x01, nullptr, 0)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket(buf, sizeof(buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 3) && ((rerr & 0x7f) == 0);
    if (err != nullptr) *err = rerr;
  }
  FREE;
  return result;
}

// ping2
bool DX2LIB::Ping2(uint8_t *num, TAlarmStatus *AlarmStatus, uint8_t *err) {
  LOCK;
  uint8_t rbuf[20];
  int detectnum = 0;

  if (err != nullptr) *err = ERR_NON;
  if (*num < 1 || 254 < *num) {
    if (err != nullptr) *err = ERR_INVALID_PARAM;
  } else {
    uint32_t prevtimeout = offsettimeout;
    uint32_t timeout = offsettimeout + 1200;
    uint32_t detecttime = millis() + timeout;
    if (AlarmStatus != nullptr) {
      if (TxPacket(BROADCASTING_ID, INST_PING, nullptr /*param*/, 0)) {
        while (detecttime >= millis() && *num > 0) {
          uint8_t rid, rerr;
          uint16_t l;
          dxif->settimeout(timeout);
          if (RxPacket(rbuf, sizeof(rbuf), &rid, &l, /*timeout, */ &rerr)) {
            if ((rbuf[4] < BROADCASTING_ID) && (l == 3) && (*num > 0)) {
              AlarmStatus->id = rid;
              AlarmStatus->err = rerr;
              AlarmStatus++;
              detectnum++;
              *num = *num - 1;
            } else if (err != nullptr) *err |= ERR_TIMEOUT;
          }
        }
      }
    }
    dxif->settimeout(prevtimeout);
    *num = detectnum;
  }
  FREE;
  return detectnum > 0;
}

// factory reset
bool DX2LIB::Reset(uint8_t id, uint8_t *err) {
  if (id > 252) return false;
  LOCK;
  uint8_t buf[10] = { 0xff };
  bool result = false;
  if (TxPacket(id, 0x06, buf, 1)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket(buf, sizeof(buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x7f) == 0);
    if (err != nullptr) *err = rerr;
  }
  FREE;
  return result;
}

// reboot
bool DX2LIB::Reboot(uint8_t id, uint8_t *err) {
  if (id > 252) return false;
  LOCK;
  uint8_t buf[10];
  bool result = false;
  if (TxPacket(id, 0x08, nullptr, 0)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket(buf, sizeof(buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x7f) == 0);
    if (err != nullptr) *err = rerr;
  }
  FREE;
  return result;
}

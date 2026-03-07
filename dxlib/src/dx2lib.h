/*
  dx2lib.h
  Dynamixel Protocol V.2 Library

  Copyright (c) 2016-2026 BestTechnology CO.,LTD.
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "dxdef.h"

typedef struct {
  uint16_t addr;
  uint16_t length;
  uint8_t ids[];
} TSyncReadParam;

typedef struct {
  uint8_t id;
  uint8_t err;
} TAlarmStatus, *PAlarmStatus;

typedef struct {
  uint8_t id;
  uint16_t addr;
  uint16_t length;
} __attribute__((__packed__)) TBulkReadParam;

typedef struct {
  uint16_t size;
  uint8_t id;
  uint8_t err;
  uint8_t dat[];
} __attribute__((__packed__)) TBulkReadResult, *PBulkReadResult;

class DX2LIB {
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

  DX2LIB(const PDXHost_ConfParam param);
  ~DX2LIB();
  void begin(uint32_t baud, uint32_t timeout);
  void end(void);

  bool TxPacket(uint8_t id, uint8_t inst, uint8_t *param, uint16_t len);
  bool RxPacket(uint8_t *rdata, uint16_t rdatasize, uint8_t *rid, uint16_t *rlen, uint8_t *err = nullptr);

  bool ReadBlockData(uint8_t id, uint16_t addr, uint8_t *data, uint16_t len, uint8_t *err = nullptr);

  inline bool ReadByteData(uint8_t id, uint16_t addr, uint8_t *data, uint8_t *err = nullptr) {
    return ReadBlockData(id, addr, (uint8_t *)data, 1, err);
  }
  inline bool ReadByteData(uint8_t id, uint16_t addr, int8_t *data, uint8_t *err = nullptr) {
    return ReadBlockData(id, addr, (uint8_t *)data, 1, err);
  }
  inline bool ReadWordData(uint8_t id, uint16_t addr, uint16_t *data, uint8_t *err = nullptr) {
    return ReadBlockData(id, addr, (uint8_t *)data, 2, err);
  }
  inline bool ReadWordData(uint8_t id, uint16_t addr, int16_t *data, uint8_t *err = nullptr) {
    return ReadBlockData(id, addr, (uint8_t *)data, 2, err);
  }
  inline bool ReadLongData(uint8_t id, uint16_t addr, uint32_t *data, uint8_t *err = nullptr) {
    uint8_t buf[5];
    if (!ReadBlockData(id, addr, buf, 4, err)) return false;
    *data = *(uint32_t *)(&buf);
    return true;
  }
  inline bool ReadLongData(uint8_t id, uint16_t addr, int32_t *data, uint8_t *err = nullptr) {
    uint8_t buf[5];
    if (!ReadBlockData(id, addr, buf, 4, err)) return false;
    *data = *(int32_t *)(&buf);
    return true;
  }

  bool WriteBlockData(uint8_t id, uint16_t addr, const uint8_t *data, uint16_t len, uint8_t *err = nullptr);

  inline bool WriteByteData(uint8_t id, uint16_t addr, uint8_t data, uint8_t *err = nullptr) {
    return WriteBlockData(id, addr, (uint8_t *)&data, 1, err);
  }
  inline bool WriteWordData(uint8_t id, uint16_t addr, uint16_t data, uint8_t *err = nullptr) {
    return WriteBlockData(id, addr, (uint8_t *)&data, 2, err);
  }
  inline bool WriteLongData(uint8_t id, uint16_t addr, uint32_t data, uint8_t *err = nullptr) {
    return WriteBlockData(id, addr, (uint8_t *)&data, 4, err);
  }

  bool WriteSyncByteData(const uint8_t *pid, uint16_t addr, const uint8_t *pdata, int num);
  inline bool WriteSyncByteData(const uint8_t *pid, uint16_t addr, const int8_t *pdata, int num) {
    return WriteSyncByteData(pid, addr, (const uint8_t *)pdata, num);
  }
  bool WriteSyncWordData(const uint8_t *pid, uint16_t addr, const uint16_t *pdata, int num);
  inline bool WriteSyncWordData(const uint8_t *pid, uint16_t addr, const int16_t *pdata, int num) {
    return WriteSyncWordData(pid, addr, (const uint16_t *)pdata, num);
  }
  bool WriteSyncLongData(const uint8_t *pid, uint16_t addr, const uint32_t *pdata, int num);
  inline bool WriteSyncLongData(const uint8_t *pid, uint16_t addr, const int32_t *pdata, int num) {
    return WriteSyncLongData(pid, addr, (const uint32_t *)pdata, num);
  }

  bool ReadSyncData(const TSyncReadParam *param, uint32_t *num, uint8_t *dat, uint8_t *err = nullptr);

  bool WriteBulkData(uint8_t *dat, uint32_t size);
  bool ReadBulkData(const TBulkReadParam *param, uint8_t *num, uint8_t *dat, uint8_t *err = nullptr);

  bool Ping(uint8_t id, uint8_t *err = nullptr);
  bool Ping2(uint8_t *num, TAlarmStatus *AlarmStatus, uint8_t *err = nullptr);

  bool Reset(uint8_t id, uint8_t *err = nullptr);
  bool Reboot(uint8_t id, uint8_t *err = nullptr);

#ifndef __AVR__
  bool SetLED(uint8_t id, bool en);
  bool SetTorqueEnable(uint8_t id, bool en);
  bool SetTorqueEnables(const uint8_t *ids, const bool *ens, int num);
  bool SetTorqueEnablesEquival(const uint8_t *ids, int num, bool en);
  bool GetTorqueEnable(uint8_t id, bool *en);
  bool GetTorqueEnables(const uint8_t *ids, bool *en, int num);
  bool SetGoalAngle(uint8_t id, double angle);
  bool SetGoalAngles(const uint8_t *ids, const double *angles, int num);
  bool GetPresentAngle(uint8_t id, double *angle);
  bool GetPresentAngles(const uint8_t *ids, double *angles, int num);
  bool StandStillAngle(uint8_t id);
  bool StandStillAngles(const uint8_t *ids, int num);
  bool SetGoalVelocity(uint8_t id, double velocity);
  bool SetGoalVelocities(const uint8_t *ids, const double *velocities, int num);
  bool GetPresentVelocity(uint8_t id, double *velocity);
  bool GetPresentVelocities(const uint8_t *ids, double *velocities, int num);
  bool SetGoalAngleAndVelocity(uint8_t id, double angle, double velocity);
  bool SetGoalAnglesAndVelocities(const uint8_t *ids, PAngleVelocity anglevelocity, int num);
  bool SetGoalAngleAndTime(uint8_t id, double angle, double sec);
  bool SetGoalAnglesAndTime(const uint8_t *ids, const double *angles, int num, double sec);
  bool SetGoalAngleAndTime2(uint8_t id, double angle, double sec);
  bool SetGoalAnglesAndTime2(const uint8_t *ids, const double *angles, int num, double sec);
  bool SetGoalCurrent(uint8_t id, double current);
  bool SetGoalCurrents(const uint8_t *ids, const double *currents, int num);
  bool GetPresentCurrent(uint8_t id, double *current);
  bool GetPresentCurrents(const uint8_t *ids, double *currents, int num);
  bool SetGoalPWM(uint8_t id, double pwm);
  bool SetGoalPWMs(const uint8_t *ids, const double *pwms, int num);
  bool GetPresentPWM(uint8_t id, double *pwm);
  bool GetPresentPWMs(const uint8_t *ids, double *pwms, int num);

  bool SetMultipleGoalValues(const uint8_t *ids, const double *values, int num, double sec);

  bool SetDriveMode(uint8_t id, uint8_t mode);
  bool SetDriveModesEquival(const uint8_t *ids, int num, uint8_t mode);
  bool SetOperatingMode(uint8_t id, uint8_t mode);
  bool SetOperatingModesEquival(const uint8_t *ids, int num, uint8_t mode);
  bool GetOperatingMode(uint8_t id, uint8_t *mode);
  bool GetHWErrorCode(uint8_t id, uint8_t *hwerr);
  TErrorCode GetErrorCode(uint8_t id);
  PDXL_ModelInfo GetModelInfo(uint8_t id);
  PDXL_ModelInfo GetStoredModelInfoByID(uint8_t id);

  int ScanDevices(uint8_t *ids);
  bool PrintDevicesList(int (*pf)(const char *, ...));
  void InitDevicesList(void);
#endif


private:
  PDXHost_ConfParam dxif;
  uint32_t baudrate;
  uint32_t offsettimeout;
  bool active(void);
  uint16_t crc16(uint16_t *crc, const uint8_t *dat, uint16_t size);
  uint16_t addsuffix(const uint8_t *dat, uint16_t len, bool tx, uint16_t *crc);
  double CalcTimeout(uint32_t baud, uint32_t num);

#ifndef __AVR__
  static const TDXL_ModelInfo ModelInfoList[];

  struct TDevices {
    uint8_t num;
    uint8_t id2model[256];
    uint8_t drivemode[256];
    uint8_t opmode[256];
    //  TErrorCode  err[256];
  } __attribute__((__packed__)) Devices;

  void initarray(void);
  int32_t ang2pos(double angle, const TDXL_ModelInfo *m);
  double pos2ang(int32_t position, const TDXL_ModelInfo *m);
#endif
};
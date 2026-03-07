#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define BROADCASTING_ID (0xfe)

  // Error flags
  typedef uint16_t TErrorCode;

#define ERR_INVALID_ID (7 << 3)
#define ERR_DIFF_ID (6 << 3)
#define ERR_ILLEGAL_SIZE (5 << 3)
#define ERR_INVALID_PARAM (4 << 3)
#define ERR_CHECKSUM (3 << 3)
#define ERR_TIMEOUT (2 << 3)
#define ERR_DX2_ALERT (1 << 7)
#define ERR_DX2_ACCESS (7)
#define ERR_DX2_DATALIMIT (6)
#define ERR_DX2_DATALENGTH (5)
#define ERR_DX2_DATARANGE (4)
#define ERR_DX2_CRC (3)
#define ERR_DX2_INSTRUCTION (2)
#define ERR_DX2_RESULT (1)
#define ERR_NON (0)


  // インストラクション
  typedef enum {
    INST_PING = 0x01,
    INST_READ = 0x02,
    INST_WRITE = 0x03,
    INST_REG_WRITE = 0x04,
    INST_ACTION = 0x05,
    INST_RESET = 0x06,
    INST_REBOOT = 0x08,
    INST_SYS_WRITE = 0x0d,
    INST_CLEAR = 0x10,
    INST_CONTROL_TABLE_BACKUP = 0x20,
    INST_STATUS = 0x55,
    INST_SYNC_READ = 0x82,
    INST_SYNC_WRITE = 0x83,
    INST_SYNC_REG_WRITE = 0x85,
    INST_FAST_SYNC_READ = 0x8a,
    INST_BULK_READ = 0x92,
    INST_BULK_WRITE = 0x93,
    INST_FAST_BULK_READ = 0x9a,
  } TDXInstruction;

  typedef uint16_t TDXErrCode;

  // デバイスの種別
  typedef enum {
    devtNONE,
    devtDX,
    devtAX,
    devtRX,
    devtEX,
    devtMX,
    devtXL320,
    devtPRO,
    devtPROP,
    devtX,
    devtY
  } TDXL_DevType;

  // 全シリーズモデル情報
  typedef struct {
    uint16_t modelno;  // デバイス固有のモデル番号
    char name[18];
    TDXL_DevType devtype;  // デバイスのグループ
    struct {               // 位置の範囲
      int32_t max;
      int32_t min;
    } __attribute__((__packed__)) positionlimit;
    struct {  // 角度の範囲
      double max;
      double min;
    } __attribute__((__packed__)) anglelimit;
    struct {  // 速度の範囲
      int32_t max;
      int32_t min;
    } __attribute__((__packed__)) velocitylimit;
    struct {  // PWMの範囲
      int32_t max;
      int32_t min;
    } __attribute__((__packed__)) pwmlimit;
    double velocityratio;  // 角速度変換係数 [deg/sec]
    double currentratio;   // 電流変換係数 [mA]
    double pwmratio;       // PWM変換係数 [%]
  } __attribute__((__packed__)) TDXL_ModelInfo, *PDXL_ModelInfo;

  //
  typedef struct {
    double angle;
    double velocity;
  } TAngleVelocity, *PAngleVelocity;

  typedef struct {
    double angle;
    double sec;
  } TAngleTime, *PAngleTime;

#ifdef __cplusplus
}
#endif
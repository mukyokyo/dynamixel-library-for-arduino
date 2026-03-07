// There are many types of DXLs and they are not compatible with each other.
// We have tried to make them into a general category to a certain extent.
// The basic idea is just a quote from the win version of dxlib.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/pgmspace.h>
#include "dxdef.h"
#include "dxmemmap.h"
#include "dx2memmap.h"

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#ifndef PROGMEM
#define PROGMEM
#endif
#endif

// Model Information
typedef struct {
  uint16_t      modelno;
  char          name[18];
  TDXL_DevType  devtype;
  struct {
    int32_t max;
    int32_t min;
  } positionlimit;
} __attribute__ ((__packed__)) TModelInfo, *PModelInfo;

// Item Address
typedef struct {
  uint16_t addr;
} TItemInfo;

typedef struct {
  TDXL_DevType typ;
  TItemInfo drivemode;
  TItemInfo opmode;
  TItemInfo torque_en;
  TItemInfo led;
  TItemInfo g_pos;
  TItemInfo g_velo;
  TItemInfo p_pos;
  TItemInfo p_velo;
} TMajorItemsAddr, *PMajorItemsAddr;

const TMajorItemsAddr MajorItemsAddr[] PROGMEM = {
  { devtDX,    {0},                       {0},                           {ADDRESS_TORQUE_ENABLE},     {ADDRESS_LED},          {ADDRESS_GOAL_POSITION},      {ADDRESS_GOAL_SPEED},         {ADDRESS_PRESENT_POSITION},      {ADDRESS_PRESENT_SPEED}         },
  { devtAX,    {0},                       {0},                           {ADDRESS_TORQUE_ENABLE},     {ADDRESS_LED},          {ADDRESS_GOAL_POSITION},      {ADDRESS_GOAL_SPEED},         {ADDRESS_PRESENT_POSITION},      {ADDRESS_PRESENT_SPEED}         },
  { devtRX,    {0},                       {0},                           {ADDRESS_TORQUE_ENABLE},     {ADDRESS_LED},          {ADDRESS_GOAL_POSITION},      {ADDRESS_GOAL_SPEED},         {ADDRESS_PRESENT_POSITION},      {ADDRESS_PRESENT_SPEED}         },
  { devtEX,    {0},                       {0},                           {ADDRESS_TORQUE_ENABLE},     {ADDRESS_LED},          {ADDRESS_GOAL_POSITION},      {ADDRESS_GOAL_SPEED},         {ADDRESS_PRESENT_POSITION},      {ADDRESS_PRESENT_SPEED}         },
  { devtMX,    {0},                       {0},                           {ADDRESS_TORQUE_ENABLE},     {ADDRESS_LED},          {ADDRESS_GOAL_POSITION},      {ADDRESS_GOAL_SPEED},         {ADDRESS_PRESENT_POSITION},      {ADDRESS_PRESENT_SPEED}         },
  { devtMX,    {0},                       {0},                           {ADDRESS_TORQUE_ENABLE},     {ADDRESS_LED},          {ADDRESS_GOAL_POSITION},      {ADDRESS_GOAL_SPEED},         {ADDRESS_PRESENT_POSITION},      {ADDRESS_PRESENT_SPEED}         },
  { devtXL320, {0},                       {0},                           {ADDRESS_TORQUE_ENABLE},     {ADDRESS_LED},          {ADDRESS_GOAL_POSITION},      {ADDRESS_GOAL_SPEED},         {ADDRESS_PRESENT_POSITION},      {ADDRESS_PRESENT_SPEED}         },
  { devtPRO,   {0},                       {ADDRESS_PRO_OPERATING_MODE},  {ADDRESS_PRO_TORQUE_ENABLE}, {ADDRESS_PRO_LED_RED},  {ADDRESS_PRO_GOAL_POSITION},  {ADDRESS_PRO_GOAL_VELOCITY},  {ADDRESS_PRO_PRESENT_POSITION},  {ADDRESS_PRO_PRESENT_VELOCITY}  },
  { devtPROP,  {ADDRESS_PROP_DRIVE_MODE}, {ADDRESS_PROP_OPERATING_MODE}, {ADDRESS_PROP_TORQUE_ENABLE}, {ADDRESS_PROP_LED_RED}, {ADDRESS_PROP_GOAL_POSITION}, {ADDRESS_PROP_GOAL_VELOCITY}, {ADDRESS_PROP_PRESENT_POSITION}, {ADDRESS_PROP_PRESENT_VELOCITY} },
  { devtX,     {ADDRESS_X_DRIVE_MODE},    {ADDRESS_X_OPERATING_MODE},    {ADDRESS_X_TORQUE_ENABLE},   {ADDRESS_X_LED_RED},    {ADDRESS_X_GOAL_POSITION},    {ADDRESS_X_GOAL_VELOCITY},    {ADDRESS_X_PRESENT_POSITION},    {ADDRESS_X_PRESENT_VELOCITY}    },
  { devtY,     {ADDRESS_Y_DRIVE_MODE},    {ADDRESS_Y_OPERATING_MODE},    {ADDRESS_Y_TORQUE_ENABLE},   {ADDRESS_Y_LED},        {ADDRESS_Y_GOAL_POSITION},    {ADDRESS_Y_GOAL_VELOCITY},    {ADDRESS_Y_PRESENT_POSITION},    {ADDRESS_Y_PRESENT_VELOCITY}    },
};

// Minimum information for all models
const TModelInfo ModelInfoList[] PROGMEM = {
  { 0,      "NONE",            devtNONE },
  { 0xffff, "UNKNOWN",         devtNONE },

  { 0x0071, "DX-113",          devtDX,    { 1023, 0 } },
  { 0x0074, "DX-116",          devtDX,    { 1023, 0 } },
  { 0x0075, "DX-117",          devtDX,    { 1023, 0 } },

  { 0x002C, "AX-12W",          devtAX,    { 1023, 0 } },
  { 0x000C, "AX-12",           devtAX,    { 1023, 0 } },
  { 0x0012, "AX-18",           devtAX,    { 1023, 0 } },

  { 0x000A, "RX-10",           devtRX,    { 1023, 0 } },
  { 0x0018, "RX-24F",          devtRX,    { 1023, 0 } },
  { 0x001C, "RX-28",           devtRX,    { 1023, 0 } },
  { 0x0040, "RX-64",           devtRX,    { 1023, 0 } },
  { 0x006B, "EX-106+",         devtEX,    { 4095, 0 } },

  { 0x0068, "MX-12W",          devtMX,    { 4095, 0 } },
  { 0x001D, "MX-28",           devtMX,    { 4095, 0 } },
  { 0x0136, "MX-64",           devtMX,    { 4095, 0 } },
  { 0x0140, "MX-106",          devtMX,    { 4095, 0 } },

  { 0x001E, "MX-28(2.0)",      devtX,     { 4095, 0 } },
  { 0x0137, "MX-64(2.0)",      devtX,     { 4095, 0 } },
  { 0x0141, "MX-106(2.0)",     devtX,     { 4095, 0 } },

  { 0x015E, "XL-320",          devtXL320, { 1023, 0 } },

  { 0x04A6, "XL330-M077",      devtX,     { 4095, 0 } },
  { 0x04B0, "XL330-M288",      devtX,     { 4095, 0 } },
  { 0x04CE, "XC330-M181",      devtX,     { 4095, 0 } },
  { 0x04D8, "XC330-M288",      devtX,     { 4095, 0 } },
  { 0x04BA, "XC330-T181",      devtX,     { 4095, 0 } },
  { 0x04C4, "XC330-T288",      devtX,     { 4095, 0 } },
  { 0x0424, "XL430-W250",      devtX,     { 4095, 0 } },
  { 0x0442, "2XL430-W250",     devtX,     { 4095, 0 } },
  { 0x0488, "2XC430-W250",     devtX,     { 4095, 0 } },
  { 0x042E, "XC430-W150",      devtX,     { 4095, 0 } },
  { 0x0438, "XC430-W240",      devtX,     { 4095, 0 } },
  { 0x0406, "XM430-W210",      devtX,     { 4095, 0 } },
  { 0x03F2, "XH430-W210",      devtX,     { 4095, 0 } },
  { 0x03F3, "XD430-T210",      devtX,     { 4095, 0 } },
  { 0x041A, "XH430-V210",      devtX,     { 4095, 0 } },
  { 0x03FC, "XM430-W350",      devtX,     { 4095, 0 } },
  { 0x03E8, "XH430-W350",      devtX,     { 4095, 0 } },
  { 0x03E9, "XD430-T350",      devtX,     { 4095, 0 } },
  { 0x0410, "XH430-V350",      devtX,     { 4095, 0 } },
  { 0x0500, "XW430-T200",      devtX,     { 4095, 0 } },
  { 0x04F6, "XW430-T333",      devtX,     { 4095, 0 } },
  { 0x046A, "XM540-W150",      devtX,     { 4095, 0 } },
  { 0x0456, "XH540-W150",      devtX,     { 4095, 0 } },
  { 0x0457, "XD540-T150",      devtX,     { 4095, 0 } },
  { 0x047E, "XH540-V150",      devtX,     { 4095, 0 } },
  { 0x0460, "XM540-W270",      devtX,     { 4095, 0 } },
  { 0x044C, "XH540-W270",      devtX,     { 4095, 0 } },
  { 0x044D, "XD540-T270",      devtX,     { 4095, 0 } },
  { 0x0474, "XH540-V270",      devtX,     { 4095, 0 } },
  { 0x049C, "XW540-T140",      devtX,     { 4095, 0 } },
  { 0x0492, "XW540-T260",      devtX,     { 4095, 0 } },

  { 0x8900, "L42-10-S300-R",   devtPRO,   {   2048,   -2047 } },
  { 0x9428, "L54-30-S400-R",   devtPRO,   { 144197, -144197 } },
  { 0x9408, "L54-30-S500-R",   devtPRO,   { 180692, -180692 } },
  { 0x9520, "L54-50-S290-R",   devtPRO,   { 103846, -103846 } },
  { 0x9508, "L54-50-S500-R",   devtPRO,   { 180692, -180692 } },
  { 0xA918, "M42-10-S260-R",   devtPRO,   { 131593, -131593 } },
  { 0xB410, "M54-40-S250-R",   devtPRO,   { 125708, -125708 } },
  { 0xB510, "M54-60-S250-R",   devtPRO,   { 125708, -125708 } },
  { 0xC800, "H42-20-S300-R",   devtPRO,   { 151875, -151875 } },
  { 0xD208, "H54-100-S500-R",  devtPRO,   { 250961, -250961 } },
  { 0xD308, "H54-200-S500-R",  devtPRO,   { 250961, -250961 } },
  { 0xA919, "M42-10-S260-RA",  devtPROP,  { 262931, -262931 } },
  { 0xB411, "M54-40-S250-RA",  devtPROP,  { 251173, -251173 } },
  { 0xB511, "M54-60-S250-RA",  devtPROP,  { 251173, -251173 } },
  { 0xC801, "H42-20-S300-RA",  devtPROP,  { 303454, -303454 } },
  { 0xD209, "H54-100-S500-RA", devtPROP,  { 501433, -501433 } },
  { 0xD309, "H54-200-S500-RA", devtPROP,  { 501433, -501433 } },

  { 0x0834, "PM42-010-S260-R", devtPROP,  { 262931, -262931 } },
  { 0x083E, "PM54-040-S250-R", devtPROP,  { 251173, -251173 } },
  { 0x0848, "PM54-060-S250-R", devtPROP,  { 251173, -251173 } },
  { 0x07D0, "PH42-020-S300-R", devtPROP,  { 303454, -303454 } },
  { 0x07DA, "PH54-100-S500-R", devtPROP,  { 501433, -501433 } },
  { 0x07E4, "PH54-200-S500-R", devtPROP,  { 501433, -501433 } },

  { 0x0FA0, "YM070-210-M001-RH", devtY,   { 262144, -262144 } },
  { 0x0FAA, "YM070-210-B001-RH", devtY,   { 262144, -262144 } },
  { 0x0FB4, "YM070-210-R051-RH", devtY,   { 262144, -262144 } },
  { 0x0FC8, "YM070-210-A051-RH", devtY,   { 262144, -262144 } },
  { 0x0FBE, "YM070-210-R099-RH", devtY,   { 262144, -262144 } },
  { 0x0FD2, "YM070-210-A099-RH", devtY,   { 262144, -262144 } },

  { 0x1018, "YM080-230-M001-RH", devtY,   { 262144, -262144 } },
  { 0x1022, "YM080-230-B001-RH", devtY,   { 262144, -262144 } },
  { 0x102C, "YM080-230-R051-RH", devtY,   { 262144, -262144 } },
  { 0x1040, "YM080-230-A051-RH", devtY,   { 262144, -262144 } },
  { 0x1036, "YM080-230-R099-RH", devtY,   { 262144, -262144 } },
  { 0x104A, "YM080-230-A099-RH", devtY,   { 262144, -262144 } },
};

// Obtaining a pointer to model information
// *Since handling PROGMEM is troublesome, we are cheating here with a function.
PModelInfo whoareyou (uint16_t model) {
  static TModelInfo info;
  memcpy_P (&info, &ModelInfoList[0], sizeof (TModelInfo));
  for (int i = 2; i < (sizeof (ModelInfoList) / sizeof (ModelInfoList[0])); i++) {
    if (pgm_read_word (&ModelInfoList[i].modelno) == model) {
      memcpy_P (&info, &ModelInfoList[i], sizeof (TModelInfo));
      break;
    }
  }
  return &info;
}

// Obtaining model address information
PMajorItemsAddr itemaddr (uint16_t model) {
  static TMajorItemsAddr m;
  TDXL_DevType  devtype = whoareyou (model)->devtype;
  for (int i = 0; i < (sizeof (MajorItemsAddr) / sizeof (MajorItemsAddr[0])); i++) {
    if (pgm_read_byte (&MajorItemsAddr[i].typ) == devtype) {
      memcpy_P (&m, &MajorItemsAddr[i], sizeof (TMajorItemsAddr));
      break;
    }
  }
  return &m;
}

#ifdef __cplusplus
}
#endif
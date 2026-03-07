/*
  dx2lib.cpp
  Dynamixel Protocol V.2 Intuitive Library

  Copyright (c) 2016-2026 BestTechnology CO.,LTD.
*/

#ifndef __AVR__

#include  <stdlib.h>
#include  <malloc.h>
#include  <string.h>
#include  <math.h>
#include  "dx2lib.h"
#include  "dx2memmap.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

const TDXL_ModelInfo DX2LIB::ModelInfoList[] = {
  { 0,      "NONE",             devtNONE },  // indefinite
  { 0xffff, "UNKNOWN",          devtNONE },  // not on the list

  // mode no, name,             type,      position range,        angle range,    velocity range,    pwm range,     angle coefficient,   current coefficient,       pwm coefficient
  { 0x015E, "XL-320",           devtXL320, {    1023,        0 }, {  150, -150 }, {  1023,  -1023 }, {    0,    0 },         0.111 * 6.0,  1100.0 / 1023.0 * 1.000,             0 },

  { 0x001E, "MX-28(2.0)",       devtX,     {    4095,        0 }, {  180, -180 }, {   230,   -230 }, {  885, -885 },         0.229 * 6.0,  1400.0 / 1000.0 * 1.000, 100.0 / 885.0 },
  { 0x0137, "MX-64(2.0)",       devtX,     {    4095,        0 }, {  180, -180 }, {   285,   -285 }, {  885, -885 },         0.229 * 6.0,             3.36 * 1.000, 100.0 / 885.0 },
  { 0x0141, "MX-106(2.0)",      devtX,     {    4095,        0 }, {  180, -180 }, {   210,   -210 }, {  885, -885 },         0.229 * 6.0,             3.36 * 1.000, 100.0 / 885.0 },
  { 0x04A6, "XL330-M077",       devtX,     {    4095,        0 }, {  180, -180 }, {  1620,  -1620 }, {  885, -885 },         0.229 * 6.0,                      1.0, 100.0 / 885.0 },
  { 0x04B0, "XL330-M288",       devtX,     {    4095,        0 }, {  180, -180 }, {   445,   -445 }, {  885, -885 },         0.229 * 6.0,                      1.0, 100.0 / 885.0 },
  { 0x04CE, "XC330-M181",       devtX,     {    4095,        0 }, {  180, -180 }, {  2047,  -2047 }, {  885, -885 },         0.229 * 6.0,                      1.0, 100.0 / 885.0 },
  { 0x04D8, "XC330-M288",       devtX,     {    4095,        0 }, {  180, -180 }, {  2047,  -2047 }, {  885, -885 },         0.229 * 6.0,                      1.0, 100.0 / 885.0 },
  { 0x04BA, "XC330-T181",       devtX,     {    4095,        0 }, {  180, -180 }, {  2047,  -2047 }, {  885, -885 },         0.229 * 6.0,                      1.0, 100.0 / 885.0 },
  { 0x04C4, "XC330-T288",       devtX,     {    4095,        0 }, {  180, -180 }, {  2047,  -2047 }, {  885, -885 },         0.229 * 6.0,                      1.0, 100.0 / 885.0 },
  { 0x0424, "XL430-W250",       devtX,     {    4095,        0 }, {  180, -180 }, {   265,   -265 }, {  885, -885 },         0.229 * 6.0,  1400.0 / 1000.0 * 1.000, 100.0 / 885.0 },
  { 0x0442, "2XL430-W250",      devtX,     {    4095,        0 }, {  180, -180 }, {   250,   -250 }, {  885, -885 },         0.229 * 6.0,  1400.0 / 1000.0 * 1.000, 100.0 / 885.0 },
  { 0x0488, "2XC430-W250",      devtX,     {    4095,        0 }, {  180, -180 }, {   275,   -275 }, {  885, -885 },         0.229 * 6.0,  1400.0 / 1000.0 * 1.000, 100.0 / 885.0 },
  { 0x042E, "XC430-W150",       devtX,     {    4095,        0 }, {  180, -180 }, {   460,   -460 }, {  885, -885 },         0.229 * 6.0,  1400.0 / 1000.0 * 1.000, 100.0 / 885.0 },
  { 0x0438, "XC430-W240",       devtX,     {    4095,        0 }, {  180, -180 }, {   306,   -306 }, {  885, -885 },         0.229 * 6.0,  1400.0 / 1000.0 * 1.000, 100.0 / 885.0 },
  { 0x0406, "XM430-W210",       devtX,     {    4095,        0 }, {  180, -180 }, {   330,   -330 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x03F2, "XH430-W210",       devtX,     {    4095,        0 }, {  180, -180 }, {   210,   -210 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x03F3, "XD430-T210",       devtX,     {    4095,        0 }, {  180, -180 }, {   210,   -210 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x041A, "XH430-V210",       devtX,     {    4095,        0 }, {  180, -180 }, {   230,   -230 }, {  885, -885 },         0.229 * 6.0,             1.34 * 1.000, 100.0 / 885.0 },
  { 0x03FC, "XM430-W350",       devtX,     {    4095,        0 }, {  180, -180 }, {   200,   -200 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x03E8, "XH430-W350",       devtX,     {    4095,        0 }, {  180, -180 }, {   130,   -130 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x03E9, "XD430-T350",       devtX,     {    4095,        0 }, {  180, -180 }, {   130,   -130 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x0410, "XH430-V350",       devtX,     {    4095,        0 }, {  180, -180 }, {   135,   -135 }, {  885, -885 },         0.229 * 6.0,             1.34 * 1.000, 100.0 / 885.0 },
  { 0x0500, "XW430-T200",       devtX,     {    4095,        0 }, {  180, -180 }, {   235,   -235 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x04F6, "XW430-T333",       devtX,     {    4095,        0 }, {  180, -180 }, {   139,   -139 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x046A, "XM540-W150",       devtX,     {    4095,        0 }, {  180, -180 }, {   230,   -230 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x0456, "XH540-W150",       devtX,     {    4095,        0 }, {  180, -180 }, {   300,   -300 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x0457, "XD540-T150",       devtX,     {    4095,        0 }, {  180, -180 }, {   300,   -300 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x047E, "XH540-V150",       devtX,     {    4095,        0 }, {  180, -180 }, {   230,   -230 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x0460, "XM540-W270",       devtX,     {    4095,        0 }, {  180, -180 }, {   128,   -128 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x044C, "XH540-W270",       devtX,     {    4095,        0 }, {  180, -180 }, {   167,   -167 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x044D, "XD540-T270",       devtX,     {    4095,        0 }, {  180, -180 }, {   167,   -167 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x0474, "XH540-V270",       devtX,     {    4095,        0 }, {  180, -180 }, {   128,   -128 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x049C, "XW540-T140",       devtX,     {    4095,        0 }, {  180, -180 }, {   304,   -304 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },
  { 0x0492, "XW540-T260",       devtX,     {    4095,        0 }, {  180, -180 }, {   167,   -167 }, {  885, -885 },         0.229 * 6.0,             2.69 * 1.000, 100.0 / 885.0 },

  { 0x8900, "L42-10-S300-R",    devtPRO,   {    2048,    -2047 }, {  180, -180 }, {   400,   -400 }, {    0,    0 },   1.0 / 303.8 * 6.0 * 36.0, 8250.0 / 2048.0 * 1000,        0 }, //  8476.019999999993
  { 0x9428, "L54-30-S400-R",    devtPRO,   {  144197,  -144197 }, {  180, -180 }, {  9000,  -9000 }, {    0,    0 }, 1.0 / 400.550 * 6.0, 33000.0 / 2048.0 * 1.000,             0 }, // 11495.785
  { 0x9408, "L54-30-S500-R",    devtPRO,   {  180692,  -180692 }, {  180, -180 }, {  9000,  -9000 }, {    0,    0 }, 1.0 / 501.923 * 6.0, 33000.0 / 2048.0 * 1.000,             0 }, // 11544.2290000002
  { 0x9520, "L54-50-S290-R",    devtPRO,   {  103846,  -103846 }, {  180, -180 }, {  8000,  -8000 }, {    0,    0 }, 1.0 / 288.461 * 6.0, 33000.0 / 2048.0 * 1.000,             0 }, //  7499.985999999907
  { 0x9508, "L54-50-S500-R",    devtPRO,   {  180692,  -180692 }, {  180, -180 }, {  8000,  -8000 }, {    0,    0 }, 1.0 / 501.923 * 6.0, 33000.0 / 2048.0 * 1.000,             0 }, //  8030.768000000136
  { 0xA918, "M42-10-S260-R",    devtPRO,   {  131593,  -131593 }, {  180, -180 }, {  8000,  -8000 }, {    0,    0 }, 1.0 / 257.019 * 6.0,  8250.0 / 2048.0 * 1.000,             0 }, //  7196.532000000086
  { 0xB410, "M54-40-S250-R",    devtPRO,   {  125708,  -125708 }, {  180, -180 }, {  8000,  -8000 }, {    0,    0 }, 1.0 / 251.417 * 6.0, 33000.0 / 2048.0 * 1.000,             0 }, //  7140.242799999926
  { 0xB510, "M54-60-S250-R",    devtPRO,   {  125708,  -125708 }, {  180, -180 }, {  8000,  -8000 }, {    0,    0 }, 1.0 / 251.417 * 6.0, 33000.0 / 2048.0 * 1.000,             0 }, //  8321.902699999914
  { 0xC800, "H42-20-S300-R",    devtPRO,   {  151875,  -151875 }, {  180, -180 }, { 10300, -10300 }, {    0,    0 }, 1.0 / 303.750 * 6.0,  8250.0 / 2048.0 * 1.000,             0 }, //  9932.625000000144
  { 0xD208, "H54-100-S500-R",   devtPRO,   {  250961,  -250961 }, {  180, -180 }, { 17000, -17000 }, {    0,    0 }, 1.0 / 501.923 * 6.0, 33000.0 / 2048.0 * 1.000,             0 }, // 16714.03590000028
  { 0xD308, "H54-200-S500-R",   devtPRO,   {  250961,  -250961 }, {  180, -180 }, { 17000, -17000 }, {    0,    0 }, 1.0 / 501.923 * 6.0, 33000.0 / 2048.0 * 1.000,             0 }, // 16613.65130000028
  { 0xA919, "M42-10-S260-RA",   devtPROP,  {  262931,  -262931 }, {  180, -180 }, {  2600,  -2600 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0xB411, "M54-40-S250-RA",   devtPROP,  {  251173,  -251173 }, {  180, -180 }, {  2840,  -2840 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0xB511, "M54-60-S250-RA",   devtPROP,  {  251173,  -251173 }, {  180, -180 }, {  2830,  -2830 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0xC801, "H42-20-S300-RA",   devtPROP,  {  303454,  -303454 }, {  180, -180 }, {  2920,  -2920 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0xD209, "H54-100-S500-RA",  devtPROP,  {  501433,  -501433 }, {  180, -180 }, {  2920,  -2920 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0xD309, "H54-200-S500-RA",  devtPROP,  {  501433,  -501433 }, {  180, -180 }, {  2900,  -2900 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },

  { 0x0834, "PM42-010-S260-R",  devtPROP,  {  262931,  -262931 }, {  180, -180 }, {  2600,  -2600 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0x083E, "PM54-040-S250-R",  devtPROP,  {  251173,  -251173 }, {  180, -180 }, {  2840,  -2840 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0x0848, "PM54-060-S250-R",  devtPROP,  {  251173,  -251173 }, {  180, -180 }, {  2830,  -2830 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0x07D0, "PH42-020-S300-R",  devtPROP,  {  303454,  -303454 }, {  180, -180 }, {  2920,  -2920 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0x07DA, "PH54-100-S500-R",  devtPROP,  {  501433,  -501433 }, {  180, -180 }, {  2920,  -2920 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },
  { 0x07E4, "PH54-200-S500-R",  devtPROP,  {  501433,  -501433 }, {  180, -180 }, {  2900,  -2900 }, { 2009, -2009 },          0.01 * 6.0,                      1.0, 100.0 / 2009 },

  { 0x0FA0, "YM070-210-M001-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {642200, -642200}, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x0FAA, "YM070-210-B001-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {642200, -642200}, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x0FB4, "YM070-210-R051-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, { 12592, -12592 }, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x0FC8, "YM070-210-A051-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, { 12592, -12592 }, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x0FBE, "YM070-210-R099-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {  6486,  -6486 }, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x0FD2, "YM070-210-A099-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {  6486,  -6486 }, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },

  { 0x1018, "YM080-230-M001-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {355600, -355600}, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x1022, "YM080-230-B001-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {355600, -355600}, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x102C, "YM080-230-R051-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {  6972,  -6972 }, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x1040, "YM080-230-A051-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {  6972,  -6972 }, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x1036, "YM080-230-R099-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {  3591,  -3591 }, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
  { 0x104A, "YM080-230-A099-RH", devtY,    {  262144,  -262144 }, {  180, -180 }, {  3591,  -3591 }, { 1000, -1000 },          0.01 * 6.0,                     10.0,          0.1 },
};

// Initialize all device information
void DX2LIB::initarray (void) {
  static bool init = false;
  if (!init) {
    InitDevicesList();
    init = true;
  }
}

// Convert angle to position
int32_t DX2LIB::ang2pos (double angle, const TDXL_ModelInfo *m) {
  return (angle - m->anglelimit.min) * (m->positionlimit.max - m->positionlimit.min) / (m->anglelimit.max - m->anglelimit.min) + m->positionlimit.min;
}

// Convert position to angle
double DX2LIB::pos2ang (int32_t position, const TDXL_ModelInfo *m) {
  return (position - m->positionlimit.min) * (m->anglelimit.max - m->anglelimit.min) / (m->positionlimit.max - m->positionlimit.min) + m->anglelimit.min;
}

//-------------------------------------------------
// Flickering LED
//-------------------------------------------------
bool DX2LIB::SetLED (uint8_t id, bool en) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      switch (ModelInfoList[idx].devtype) {
        case devtXL320:
          return WriteByteData (id, 25, en ? 1 : 0, NULL/*&Devices.err[id]*/);
          break;
        case devtX:
          return WriteByteData (id, ADDRESS_X_LED_RED, en ? 1 : 0, NULL/*&Devices.err[id]*/);
          break;
        case devtPRO:
          return WriteByteData (id, ADDRESS_PRO_LED_RED, en ? 255 : 0, NULL/*&Devices.err[id]*/);
          break;
        case devtPROP:
          return WriteByteData (id, ADDRESS_PROP_LED_RED, en ? 255 : 0, NULL/*&Devices.err[id]*/);
          break;
        case devtY:
          return WriteByteData (id, ADDRESS_Y_LED, en ? 1 : 0, NULL/*&Devices.err[id]*/);
          break;
        default:
          break;
      }
    }
  }
  return false;
}

//-------------------------------------------------
// Set Torque Enable
//-------------------------------------------------
bool DX2LIB::SetTorqueEnable (uint8_t id, bool en) {
  initarray();
  if (id <= 252) {
    uint8_t ren;
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      switch (ModelInfoList[idx].devtype) {
        case devtXL320:
          if (WriteByteData (id, 24, en ? 1 : 0, NULL/*&Devices.err[id]*/)) {
            if (ReadByteData (id, 24, &ren, NULL/*&Devices.err[id]*/)) {
              return (((ren == 1) ? true : false) == en);
            }
          }
          break;
        case devtX:
          if (WriteByteData (id, ADDRESS_X_TORQUE_ENABLE, en ? 1 : 0, NULL/*&Devices.err[id]*/)) {
            if (ReadByteData (id, ADDRESS_X_TORQUE_ENABLE, &ren, NULL/*&Devices.err[id]*/)) {
              return (((ren == 1) ? true : false) == en);
            }
          }
          break;
        case devtPRO:
          if (WriteByteData (id, ADDRESS_PRO_TORQUE_ENABLE, en ? 1 : 0, NULL/*&Devices.err[id]*/)) {
            if (ReadByteData (id, ADDRESS_PRO_TORQUE_ENABLE, &ren, NULL/*&Devices.err[id]*/)) {
              return (((ren == 1) ? true : false) == en);
            }
          }
          break;
        case devtPROP:
          if (WriteByteData (id, ADDRESS_PROP_TORQUE_ENABLE, en ? 1 : 0, NULL/*&Devices.err[id]*/)) {
            if (ReadByteData (id, ADDRESS_PROP_TORQUE_ENABLE, &ren, NULL/*&Devices.err[id]*/)) {
              return (((ren == 1) ? true : false) == en);
            }
          }
          break;
        case devtY:
          if (WriteByteData (id, ADDRESS_Y_TORQUE_ENABLE, en ? 1 : 0, NULL/*&Devices.err[id]*/)) {
            if (ReadByteData (id, ADDRESS_Y_TORQUE_ENABLE, &ren, NULL/*&Devices.err[id]*/)) {
              return (((ren == 1) ? true : false) == en);
            }
          }
          break;
        default:
          break;
      }
    }
  }
  return false;
}

bool DX2LIB::SetTorqueEnables (const uint8_t *ids, const bool *ens, int num) {
  struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint8_t   TorqueEnable;
  } __attribute__ ((__packed__)) Bulk[num];
  bool result = false;
  int bytes = 0;
  initarray();
  if (ens != NULL) {
    if (ids != NULL && ens != NULL) {
      for (int i = 0; i < num; i++) {
        uint8_t id = ids[i];
        if (id <= 252) {
          int idx = Devices.id2model[id];
          if (idx >= 2) {
            switch (ModelInfoList[idx].devtype) {
              case devtXL320:
                Bulk[bytes].id = id;
                Bulk[bytes].addr = 24;
                Bulk[bytes].size = 1;
                Bulk[bytes].TorqueEnable = ens[i];
                bytes++;
                break;
              case devtX:
                Bulk[bytes].id = id;
                Bulk[bytes].addr = ADDRESS_X_TORQUE_ENABLE;
                Bulk[bytes].size = 1;
                Bulk[bytes].TorqueEnable = ens[i];
                bytes++;
                break;
              case devtPRO:
                Bulk[bytes].id = id;
                Bulk[bytes].addr = ADDRESS_PRO_TORQUE_ENABLE;
                Bulk[bytes].size = 1;
                Bulk[bytes].TorqueEnable = ens[i];
                bytes++;
                break;
              case devtPROP:
                Bulk[bytes].id = id;
                Bulk[bytes].addr = ADDRESS_PROP_TORQUE_ENABLE;
                Bulk[bytes].size = 1;
                Bulk[bytes].TorqueEnable = ens[i];
                bytes++;
                break;
              case devtY:
                Bulk[bytes].id = id;
                Bulk[bytes].addr = ADDRESS_Y_TORQUE_ENABLE;
                Bulk[bytes].size = 1;
                Bulk[bytes].TorqueEnable = ens[i];
                bytes++;
                break;
              default:
                break;
            }
          }
        }
      }
      if (bytes > 0) {
        result = WriteBulkData ((uint8_t *)Bulk, 6 * bytes);
      }
    }
  }
  return result;
}

bool DX2LIB::SetTorqueEnablesEquival (const uint8_t *ids, int num, bool en) {
  bool ten[num];
  initarray();
  for (int i = 0; i < num; i++) ten[i] = en;
  return SetTorqueEnables (ids, ten, num);
}

//-------------------------------------------------
// Get Torque Enable
//-------------------------------------------------
bool DX2LIB::GetTorqueEnable (uint8_t id, bool *en) {
  initarray();
  if (en != NULL) {
    if (id <= 254) {
      int idx = Devices.id2model[id];
      if (idx >= 2) {
        uint8_t ren;
        switch (ModelInfoList[idx].devtype) {
          case devtXL320:
            if (ReadByteData (id, 24, &ren, NULL/*&Devices.err[id]*/)) {
              *en = (ren == 1);
              return true;
            }
            break;
          case devtX:
            if (ReadByteData (id, ADDRESS_X_TORQUE_ENABLE, &ren, NULL/*&Devices.err[id]*/)) {
              *en = (ren == 1);
              return true;
            }
            break;
          case devtPRO:
            if (ReadByteData (id, ADDRESS_PRO_TORQUE_ENABLE, &ren, NULL/*&Devices.err[id]*/)) {
              *en = (ren == 1);
              return true;
            }
            break;
          case devtPROP:
            if (ReadByteData (id, ADDRESS_PROP_TORQUE_ENABLE, &ren, NULL/*&Devices.err[id]*/)) {
              *en = (ren == 1);
              return true;
            }
          case devtY:
            if (ReadByteData (id, ADDRESS_Y_TORQUE_ENABLE, &ren, NULL/*&Devices.err[id]*/)) {
              *en = (ren == 1);
              return true;
            }
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

bool DX2LIB::GetTorqueEnables (const uint8_t *ids, bool *en, int num) {
  bool result = true;
  initarray();
  if (ids != NULL && en != NULL) {
    for (int i = 0; i < num; i++) {
      int idx = Devices.id2model[ids[i]];
      if (idx >= 2) {
        if (!GetTorqueEnable (ids[i], &en[i])) result = false;
      }
    }
  }
  return result;
}

//-------------------------------------------------
// Set angle
//-------------------------------------------------
bool DX2LIB::SetGoalAngle (uint8_t id, double angle) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      int32_t pmax, pmin;
      if (Devices.opmode[id] == 3) {
        pmax = ModelInfoList[idx].positionlimit.max;
        pmin = ModelInfoList[idx].positionlimit.min;
      } else {
        pmax = INT32_MAX;
        pmin = INT32_MIN;
      }
      int32_t pos = max (min (ang2pos (angle, &ModelInfoList[idx]), pmax), pmin);
      switch (ModelInfoList[idx].devtype) {
        case devtXL320:
          return WriteLongData (id, 30, pos, NULL/*&Devices.err[id]*/);
          break;
        case devtX:
          return WriteLongData (id, ADDRESS_X_GOAL_POSITION, pos, NULL/*&Devices.err[id]*/);
          break;
        case devtPRO:
          return WriteLongData (id, ADDRESS_PRO_GOAL_POSITION, pos, NULL/*&Devices.err[id]*/);
          break;
        case devtPROP:
          return WriteLongData (id, ADDRESS_PROP_GOAL_POSITION, pos, NULL/*&Devices.err[id]*/);
          break;
        case devtY:
          return WriteLongData (id, ADDRESS_Y_GOAL_POSITION, pos, NULL/*&Devices.err[id]*/);
          break;
        default:
          break;
      }
    }
  }
  return false;
}

bool DX2LIB::SetGoalAngles (const uint8_t *ids, const double *angles, int num) {
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint16_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkXL320;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkX;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkPRO;
  uint8_t *data = (uint8_t *)malloc (sizeof (TBulkPRO) * num);
  bool result = false;
  int bytes = 0;
  initarray();
  if (ids != NULL && angles != NULL) {
    for (int i = 0; i < num; i++) {
      uint8_t id = ids[i];
      if (id <= 252) {
        int idx = Devices.id2model[id];
        if (idx >= 2) {
          int32_t pmax, pmin;
          if (Devices.opmode[id] == 3) {
            pmax = ModelInfoList[idx].positionlimit.max;
            pmin = ModelInfoList[idx].positionlimit.min;
          } else {
            pmax = INT32_MAX;
            pmin = INT32_MIN;
          }
          int32_t pos = max (min (ang2pos (angles[i], &ModelInfoList[idx]), pmax), pmin);
          switch (ModelInfoList[idx].devtype) {
            case devtXL320:
              ((TBulkXL320 *) (&data[bytes]))->id = id;
              ((TBulkXL320 *) (&data[bytes]))->addr = 30;
              ((TBulkXL320 *) (&data[bytes]))->size = 2;
              ((TBulkXL320 *) (&data[bytes]))->GoalPosition = pos;
              bytes += sizeof (TBulkXL320);
              break;
            case devtX:
              ((TBulkX *) (&data[bytes]))->id = id;
              ((TBulkX *) (&data[bytes]))->addr = ADDRESS_X_GOAL_POSITION;
              ((TBulkX *) (&data[bytes]))->size = 4;
              ((TBulkX *) (&data[bytes]))->GoalPosition = pos;
              bytes += sizeof (TBulkX);
              break;
            case devtPRO:
              ((TBulkPRO *) (&data[bytes]))->id = id;
              ((TBulkPRO *) (&data[bytes]))->addr = ADDRESS_PRO_GOAL_POSITION;
              ((TBulkPRO *) (&data[bytes]))->size = 4;
              ((TBulkPRO *) (&data[bytes]))->GoalPosition = pos;
              bytes += sizeof (TBulkPRO);
              break;
            case devtPROP:
              ((TBulkPRO *) (&data[bytes]))->id = id;
              ((TBulkPRO *) (&data[bytes]))->addr = ADDRESS_PROP_GOAL_POSITION;
              ((TBulkPRO *) (&data[bytes]))->size = 4;
              ((TBulkPRO *) (&data[bytes]))->GoalPosition = pos;
              bytes += sizeof (TBulkPRO);
              break;
            case devtY:
              ((TBulkPRO *) (&data[bytes]))->id = id;
              ((TBulkPRO *) (&data[bytes]))->addr = ADDRESS_Y_GOAL_POSITION;
              ((TBulkPRO *) (&data[bytes]))->size = 4;
              ((TBulkPRO *) (&data[bytes]))->GoalPosition = pos;
              bytes += sizeof (TBulkPRO);
              break;
            default:
              break;
          }
        }
      }
    }
    if (bytes > 0) result = WriteBulkData (data, bytes);
  }
  free (data);
  return result;
}

//-------------------------------------------------
// Get current angle
//-------------------------------------------------
bool DX2LIB::GetPresentAngle (uint8_t id, double *angle) {
  initarray();
  if (angle != NULL) {
    if (id <= 252) {
      int idx = Devices.id2model[id];
      if (idx >= 2) {
        switch (ModelInfoList[idx].devtype) {
          case devtXL320: {
              int16_t pos;
              if (ReadWordData (id, 37, (uint16_t *)&pos, NULL/*&Devices.err[id]*/)) {
                *angle = pos2ang (pos, &ModelInfoList[idx]);
                return true;
              }
            }
            break;
          case devtX: {
              int32_t pos;
              if (ReadLongData (id, ADDRESS_X_PRESENT_POSITION, (uint32_t *)&pos, NULL/*&Devices.err[id]*/)) {
                *angle = pos2ang (pos, &ModelInfoList[idx]);
                return true;
              }
            }
            break;
          case devtPRO: {
              int32_t pos;
              if (ReadLongData (id, ADDRESS_PRO_PRESENT_POSITION, (uint32_t *)&pos, NULL/*&Devices.err[id]*/)) {
                *angle = pos2ang (pos, &ModelInfoList[idx]);
                return true;
              }
            }
            break;
          case devtPROP: {
              int32_t pos;
              if (ReadLongData (id, ADDRESS_PROP_PRESENT_POSITION, (uint32_t *)&pos, NULL/*&Devices.err[id]*/)) {
                *angle = pos2ang (pos, &ModelInfoList[idx]);
                return true;
              }
            }
            break;
          case devtY: {
              int32_t pos;
              if (ReadLongData (id, ADDRESS_Y_PRESENT_POSITION, (uint32_t *)&pos, NULL/*&Devices.err[id]*/)) {
                *angle = pos2ang (pos, &ModelInfoList[idx]);
                return true;
              }
            }
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

bool DX2LIB::GetPresentAngles (const uint8_t *ids, double *angles, int num) {
  bool result = true;
  initarray();
  if (ids != NULL && angles != NULL) {
    for (int i = 0; i < num; i++) {
      int idx = Devices.id2model[ids[i]];
      if (idx >= 2) {
        if (!GetPresentAngle (ids[i], &angles[i])) result = false;
      }
    }
  }
  return result;
}

//-------------------------------------------------
// Immediate stop at current position
//-------------------------------------------------
bool DX2LIB::StandStillAngle (uint8_t id) {
  double pangle;
  initarray();
  if (GetPresentAngle (id, &pangle)) return SetGoalAngleAndVelocity (id, pangle, 0);
  return false;
}

bool DX2LIB::StandStillAngles (const uint8_t *ids, int num) {
  double pangles[num];
  initarray();
#if 1
  if (GetPresentAngles (ids, pangles, num)) return SetGoalAngles (ids, pangles, num);
#else
  TAngleVelocity angvelo[num];
  memset (&angvelo, 0, sizeof (angvelo));
  if (GetPresentAngles (ids, pangles, num)) {
    for (int i = 0; i < num; i++) angvelo[i].angle = pangles[i];
    return SetGoalAnglesAndVelocities (ids, angvelo, num);
  }
#endif
  return false;
}

//-------------------------------------------------
// Set angular velocity
//-------------------------------------------------
bool DX2LIB::SetGoalVelocity (uint8_t id, double velocity) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      int32_t velo = max (min (velocity / ModelInfoList[idx].velocityratio, ModelInfoList[idx].velocitylimit.max), ModelInfoList[idx].velocitylimit.min);
      switch (ModelInfoList[idx].devtype) {
        case devtXL320:
          if (velo < 0) velo = abs (velo) | 0x400;
          return WriteWordData (id, 32, velo, NULL/*&Devices.err[id]*/);
          break;
        case devtX:
          return WriteLongData (id, ADDRESS_X_GOAL_VELOCITY, velo, NULL/*&Devices.err[id]*/);
          break;
        case devtPRO:
          return WriteLongData (id, ADDRESS_PRO_GOAL_VELOCITY, velo, NULL/*&Devices.err[id]*/);
          break;
        case devtPROP:
          return WriteLongData (id, ADDRESS_PROP_GOAL_VELOCITY, velo, NULL/*&Devices.err[id]*/);
          break;
        case devtY:
          return WriteLongData (id, ADDRESS_Y_GOAL_VELOCITY, velo, NULL/*&Devices.err[id]*/);
          break;
        default:
          break;
      }
    }
  }
  return false;
}

bool DX2LIB::SetGoalVelocities (const uint8_t *ids, const double *velocities, int num) {
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint16_t  MovingSpeed;
  } __attribute__ ((__packed__)) TBulkXL320;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalVelocity;
  } __attribute__ ((__packed__)) TBulkX;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalVelocity;
  } __attribute__ ((__packed__)) TBulkPRO;
  bool result = false;
  uint8_t *data = (uint8_t *)malloc (sizeof (TBulkPRO) * num);
  int bytes = 0;
  initarray();
  if (ids != NULL && velocities != NULL) {
    for (int i = 0; i < num; i++) {
      uint8_t id = ids[i];
      if (id <= 252) {
        int idx = Devices.id2model[id];
        if (idx >= 2) {
          int32_t velo = max (min (velocities[i] / ModelInfoList[idx].velocityratio, ModelInfoList[idx].velocitylimit.max), ModelInfoList[idx].velocitylimit.min);
          switch (ModelInfoList[idx].devtype) {
            case devtXL320:
              ((TBulkXL320 *) (&data[bytes]))->id = id;
              ((TBulkXL320 *) (&data[bytes]))->addr = 32;
              ((TBulkXL320 *) (&data[bytes]))->size = 2;
              if (velo < 0) velo = abs (velo) | 0x400;
              ((TBulkXL320 *) (&data[bytes]))->MovingSpeed = velo;
              bytes += sizeof (TBulkXL320);
              break;
            case devtX:
              ((TBulkX *) (&data[bytes]))->id = id;
              ((TBulkX *) (&data[bytes]))->addr = ADDRESS_X_GOAL_VELOCITY;
              ((TBulkX *) (&data[bytes]))->size = 4;
              ((TBulkX *) (&data[bytes]))->GoalVelocity = velo;
              bytes += sizeof (TBulkX);
              break;
            case devtPRO:
              ((TBulkPRO *) (&data[bytes]))->id = id;
              ((TBulkPRO *) (&data[bytes]))->addr = ADDRESS_PRO_GOAL_VELOCITY;
              ((TBulkPRO *) (&data[bytes]))->size = 4;
              ((TBulkPRO *) (&data[bytes]))->GoalVelocity = velo;
              bytes += sizeof (TBulkPRO);
              break;
            case devtPROP:
              ((TBulkPRO *) (&data[bytes]))->id = id;
              ((TBulkPRO *) (&data[bytes]))->addr = ADDRESS_PROP_GOAL_VELOCITY;
              ((TBulkPRO *) (&data[bytes]))->size = 4;
              ((TBulkPRO *) (&data[bytes]))->GoalVelocity = velo;
              bytes += sizeof (TBulkPRO);
              break;
            case devtY:
              ((TBulkPRO *) (&data[bytes]))->id = id;
              ((TBulkPRO *) (&data[bytes]))->addr = ADDRESS_Y_GOAL_VELOCITY;
              ((TBulkPRO *) (&data[bytes]))->size = 4;
              ((TBulkPRO *) (&data[bytes]))->GoalVelocity = velo;
              bytes += sizeof (TBulkPRO);
              break;
            default:
              break;
          }
        }
      }
    }
    if (bytes > 0) result = WriteBulkData (data, bytes);
  }
  free (data);
  return result;
}

//-------------------------------------------------
// Get current angular velocity
//-------------------------------------------------
bool DX2LIB::GetPresentVelocity (uint8_t id, double *velocity) {
  initarray();
  if (velocity != NULL) {
    if (id <= 252) {
      int idx = Devices.id2model[id];
      if (idx >= 2) {
        switch (ModelInfoList[idx].devtype) {
          case devtXL320: {
              int16_t velo;
              if (ReadWordData (id, 39, (uint16_t *)&velo, NULL/*&Devices.err[id]*/)) {
                *velocity = (double)velo * ModelInfoList[idx].velocityratio;
                return true;
              }
            }
            break;
          case devtX: {
              int32_t velo;
              if (ReadLongData (id, ADDRESS_X_PRESENT_VELOCITY, (uint32_t *)&velo, NULL/*&Devices.err[id]*/)) {
                *velocity = (double)velo * ModelInfoList[idx].velocityratio;
                return true;
              }
            }
            break;
          case devtPRO: {
              int32_t velo;
              if (ReadLongData (id, ADDRESS_PRO_PRESENT_VELOCITY, (uint32_t *)&velo, NULL/*&Devices.err[id]*/)) {
                *velocity = (double)velo * ModelInfoList[idx].velocityratio;
                return true;
              }
            }
            break;
          case devtPROP: {
              int32_t velo;
              if (ReadLongData (id, ADDRESS_PROP_PRESENT_VELOCITY, (uint32_t *)&velo, NULL/*&Devices.err[id]*/)) {
                *velocity = (double)velo * ModelInfoList[idx].velocityratio;
                return true;
              }
            }
            break;
          case devtY: {
              int32_t velo;
              if (ReadLongData (id, ADDRESS_Y_PRESENT_VELOCITY, (uint32_t *)&velo, NULL/*&Devices.err[id]*/)) {
                *velocity = (double)velo * ModelInfoList[idx].velocityratio;
                return true;
              }
            }
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

bool DX2LIB::GetPresentVelocities (const uint8_t *ids, double *velocities, int num) {
  bool result = true;
  initarray();
  if (ids != NULL && velocities != NULL) {
    for (int i = 0; i < num; i++) {
      int idx = Devices.id2model[ids[i]];
      if (idx >= 2) {
        if (!GetPresentVelocity (ids[i], &velocities[i])) result = false;
      }
    }
  }
  return result;
}

//-------------------------------------------------
// Angle and angular velocity commands
//-------------------------------------------------
bool DX2LIB::SetGoalAngleAndVelocity (uint8_t id, double angle, double velocity) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      int32_t pmax, pmin;
      if (Devices.opmode[id] == 3) {
        pmax = ModelInfoList[idx].positionlimit.max;
        pmin = ModelInfoList[idx].positionlimit.min;
      } else {
        pmax = INT32_MAX;
        pmin = INT32_MIN;
      }
      int32_t pos = max (min (ang2pos (angle, &ModelInfoList[idx]), pmax), pmin);
      int32_t velo = abs (max (min (velocity / ModelInfoList[idx].velocityratio, ModelInfoList[idx].velocitylimit.max), ModelInfoList[idx].velocitylimit.min));
      switch (ModelInfoList[idx].devtype) {
        case devtXL320: {
            struct {
              int16_t GoalPosition;
              int16_t MovingSpeed;
            } __attribute__ ((__packed__)) block;
            block.GoalPosition = pos;
            if (velo < 0) velo = velo | 0x400;
            block.MovingSpeed = velo;
            return WriteBlockData (id, 30, (uint8_t *)&block, 4, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtX: {
            struct {
              int32_t ProfileVelocity;
              int32_t GoalPosition;
            } __attribute__ ((__packed__)) block;
            block.ProfileVelocity = velo;
            block.GoalPosition = pos;
            return WriteBlockData (id, ADDRESS_X_PROF_VELOCITY, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtPRO: {
            struct {
              int32_t GoalPosition;
              int32_t ProfileVelocity;
            } __attribute__ ((__packed__)) block;
            block.ProfileVelocity = velo;
            block.GoalPosition = pos;
            return WriteBlockData (id, ADDRESS_PRO_GOAL_POSITION, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtPROP: {
            struct {
              int32_t ProfileVelocity;
              int32_t GoalPosition;
            } __attribute__ ((__packed__)) block;
            block.ProfileVelocity = velo;
            block.GoalPosition = pos;
            return WriteBlockData (id, ADDRESS_PROP_PROF_VELOCITY, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtY: {
            struct {
              int32_t ProfileVelocity;
              int32_t GoalPosition;
            } __attribute__ ((__packed__)) block;
            block.ProfileVelocity = velo;
            block.GoalPosition = pos;
            return WriteBlockData (id, 634, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
          }
          break;
        default:
          break;
      }
    }
  }
  return false;
}

bool DX2LIB::SetGoalAnglesAndVelocities (const uint8_t *ids, PAngleVelocity anglevelocity, int num) {
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint16_t  GoalPosition;
    uint16_t  MovingSpeed;
  } __attribute__ ((__packed__)) TBulkXL320;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkX;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalPosition;
    uint32_t  GoalVelocity;
  } __attribute__ ((__packed__)) TBulkPRO;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkPROP;
  bool result = false;
  uint8_t *data = (uint8_t *)malloc (sizeof (TBulkPROP) * num);
  int bytes = 0;
  initarray();
  if (ids != NULL && anglevelocity != NULL) {
    for (int i = 0; i < num; i++) {
      uint8_t id = ids[i];
      if (id <= 252) {
        int idx = Devices.id2model[id];
        if (idx >= 2) {
          int32_t pmax, pmin;
          if (Devices.opmode[id] == 3) {
            pmax = ModelInfoList[idx].positionlimit.max;
            pmin = ModelInfoList[idx].positionlimit.min;
          } else {
            pmax = INT32_MAX;
            pmin = INT32_MIN;
          }
          int32_t pos = max (min (ang2pos (anglevelocity[i].angle, &ModelInfoList[idx]), pmax), pmin);
          int32_t velo = abs (max (min (anglevelocity[i].velocity / ModelInfoList[idx].velocityratio, ModelInfoList[idx].velocitylimit.max), ModelInfoList[idx].velocitylimit.min));
          switch (ModelInfoList[idx].devtype) {
            case devtXL320:
              ((TBulkXL320 *) (&data[bytes]))->id = id;
              ((TBulkXL320 *) (&data[bytes]))->addr = 30;
              ((TBulkXL320 *) (&data[bytes]))->size = 4;
              ((TBulkXL320 *) (&data[bytes]))->GoalPosition = pos;
              if (velo < 0) velo = velo | 0x400;
              ((TBulkXL320 *) (&data[bytes]))->MovingSpeed = velo;
              bytes += sizeof (TBulkXL320);
              break;
            case devtX:
              ((TBulkX *) (&data[bytes]))->id = id;
              ((TBulkX *) (&data[bytes]))->addr = ADDRESS_X_PROF_VELOCITY;
              ((TBulkX *) (&data[bytes]))->size = 8;
              ((TBulkX *) (&data[bytes]))->ProfileVelocity = velo;
              ((TBulkX *) (&data[bytes]))->GoalPosition = pos;
              bytes += sizeof (TBulkX);
              break;
            case devtPRO:
              ((TBulkPRO *) (&data[bytes]))->id = id;
              ((TBulkPRO *) (&data[bytes]))->addr = ADDRESS_PRO_GOAL_POSITION;
              ((TBulkPRO *) (&data[bytes]))->size = 8;
              ((TBulkPRO *) (&data[bytes]))->GoalPosition = pos;
              ((TBulkPRO *) (&data[bytes]))->GoalVelocity = velo;
              bytes += sizeof (TBulkPRO);
              break;
            case devtPROP:
              ((TBulkPROP *) (&data[bytes]))->id = id;
              ((TBulkPROP *) (&data[bytes]))->addr = ADDRESS_PROP_PROF_VELOCITY;
              ((TBulkPROP *) (&data[bytes]))->size = 8;
              ((TBulkPROP *) (&data[bytes]))->ProfileVelocity = velo;
              ((TBulkPROP *) (&data[bytes]))->GoalPosition = pos;
              bytes += sizeof (TBulkPROP);
              break;
            case devtY:
              ((TBulkPROP *) (&data[bytes]))->id = id;
              ((TBulkPROP *) (&data[bytes]))->addr = 634;
              ((TBulkPROP *) (&data[bytes]))->size = 8;
              ((TBulkPROP *) (&data[bytes]))->ProfileVelocity = velo;
              ((TBulkPROP *) (&data[bytes]))->GoalPosition = pos;
              bytes += sizeof (TBulkPROP);
              break;
            default:
              break;
          }
        }
      }
    }
    if (bytes > 0) result = WriteBulkData (data, bytes);
  }
  free (data);
  return result;
}

//-------------------------------------------------
// Angle and time commands
//-------------------------------------------------
bool DX2LIB::SetGoalAngleAndTime (uint8_t id, double angle, double sec) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      int32_t pmax, pmin;
      if (Devices.opmode[id] == 3) {
        pmax = ModelInfoList[idx].positionlimit.max;
        pmin = ModelInfoList[idx].positionlimit.min;
      } else {
        pmax = INT32_MAX;
        pmin = INT32_MIN;
      }
      double pangle;
      if (GetPresentAngle (id, &pangle)) {
        double velocity = fabs ((pangle - angle) / sec);
        int32_t pos = max (min (ang2pos (angle, &ModelInfoList[idx]), pmax), pmin);
        int32_t velo = abs (max (min (velocity / ModelInfoList[idx].velocityratio, ModelInfoList[idx].velocitylimit.max), ModelInfoList[idx].velocitylimit.min));
        switch (ModelInfoList[idx].devtype) {
          case devtXL320: {
              struct {
                int16_t GoalPosition;
                int16_t MovingSpeed;
              } __attribute__ ((__packed__)) block;
              block.GoalPosition = pos;
              if (velo < 0) velo = velo | 0x400;
              block.MovingSpeed = velo;
              return WriteBlockData (id, 30, (uint8_t *)&block, 4, NULL/*&Devices.err[id]*/);
            }
            break;
          case devtX: {
              struct {
                int32_t ProfileVelocity;
                int32_t GoalPosition;
              } __attribute__ ((__packed__)) block;
              block.ProfileVelocity = velo;
              block.GoalPosition = pos;
              return WriteBlockData (id, ADDRESS_X_PROF_VELOCITY, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
            }
            break;
          case devtPRO: {
              struct {
                int32_t GoalPosition;
                int32_t GoalVelocity;
              } __attribute__ ((__packed__)) block;
              block.GoalVelocity = abs (velo);
              block.GoalPosition = pos;
              return WriteBlockData (id, ADDRESS_PRO_GOAL_POSITION, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
            }
            break;
          case devtPROP: {
              struct {
                int32_t ProfileVelocity;
                int32_t GoalPosition;
              } __attribute__ ((__packed__)) block;
              block.ProfileVelocity = abs (velo);
              block.GoalPosition = pos;
              return WriteBlockData (id, ADDRESS_PROP_PROF_VELOCITY, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
            }
            break;
          case devtY: {
              struct {
                int32_t ProfileVelocity;
                int32_t GoalPosition;
              } __attribute__ ((__packed__)) block;
              block.ProfileVelocity = abs (velo);
              block.GoalPosition = pos;
              return WriteBlockData (id, 634, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
            }
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

bool DX2LIB::SetGoalAnglesAndTime (const uint8_t *ids, const double *angles, int num, double sec) {
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint16_t  GoalPosition;
    uint16_t  MovingSpeed;
  } __attribute__ ((__packed__)) TBulkXL320;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkX;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalPosition;
    uint32_t  GoalVelocity;
  } __attribute__ ((__packed__)) TBulkPRO;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkPROP;
  bool result = false;
  uint8_t *data = (uint8_t *)malloc (sizeof (TBulkPRO) * num);
  int bytes = 0;
  initarray();
  if (ids != NULL && angles != NULL) {
    for (int i = 0; i < num; i++) {
      uint8_t id = ids[i];
      if (id <= 252) {
        int idx = Devices.id2model[id];
        if (idx >= 2) {
          int32_t pmax, pmin;
          if (Devices.opmode[id] == 3) {
            pmax = ModelInfoList[idx].positionlimit.max;
            pmin = ModelInfoList[idx].positionlimit.min;
          } else {
            pmax = INT32_MAX;
            pmin = INT32_MIN;
          }
          double pangle;
          if (GetPresentAngle (id, &pangle)) {
            double velocity = fabs ((pangle - angles[i]) / sec);
            int32_t pos = max (min (ang2pos (angles[i], &ModelInfoList[idx]), pmax), pmin);
            int32_t velo = abs (max (min (velocity / ModelInfoList[idx].velocityratio, ModelInfoList[idx].velocitylimit.max), ModelInfoList[idx].velocitylimit.min));
            switch (ModelInfoList[idx].devtype) {
              case devtXL320:
                ((TBulkXL320 *) (&data[bytes]))->id = id;
                ((TBulkXL320 *) (&data[bytes]))->addr = 30;
                ((TBulkXL320 *) (&data[bytes]))->size = 4;
                ((TBulkXL320 *) (&data[bytes]))->GoalPosition = pos;
                if (velo < 0) velo = velo | 0x400;
                ((TBulkXL320 *) (&data[bytes]))->MovingSpeed = velo;
                bytes += sizeof (TBulkXL320);
                break;
              case devtX:
                ((TBulkX *) (&data[bytes]))->id = id;
                ((TBulkX *) (&data[bytes]))->addr = ADDRESS_X_PROF_VELOCITY;
                ((TBulkX *) (&data[bytes]))->size = 8;
                ((TBulkX *) (&data[bytes]))->ProfileVelocity = velo;
                ((TBulkX *) (&data[bytes]))->GoalPosition = pos;
                bytes += sizeof (TBulkX);
                break;
              case devtPRO:
                ((TBulkPRO *) (&data[bytes]))->id = id;
                ((TBulkPRO *) (&data[bytes]))->addr = ADDRESS_PRO_GOAL_POSITION;
                ((TBulkPRO *) (&data[bytes]))->size = 8;
                ((TBulkPRO *) (&data[bytes]))->GoalPosition = pos;
                ((TBulkPRO *) (&data[bytes]))->GoalVelocity = velo;
                bytes += sizeof (TBulkPRO);
                break;
              case devtPROP:
                ((TBulkPROP *) (&data[bytes]))->id = id;
                ((TBulkPROP *) (&data[bytes]))->addr = ADDRESS_PROP_PROF_VELOCITY;
                ((TBulkPROP *) (&data[bytes]))->size = 8;
                ((TBulkPROP *) (&data[bytes]))->ProfileVelocity = velo;
                ((TBulkPROP *) (&data[bytes]))->GoalPosition = pos;
                bytes += sizeof (TBulkPROP);
                break;
              case devtY:
                break;
              default:
                break;
            }
          }
        }
      }
    }
    if (bytes > 0) result = WriteBulkData (data, bytes);
  }
  free (data);
  return result;
}

//-------------------------------------------------
// Angle and time commands 2
//-------------------------------------------------
bool DX2LIB::SetGoalAngleAndTime2 (uint8_t id, double angle, double sec) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      int32_t pmax, pmin;
      if (Devices.opmode[id] == 3) {
        pmax = ModelInfoList[idx].positionlimit.max;
        pmin = ModelInfoList[idx].positionlimit.min;
      } else {
        pmax = INT32_MAX;
        pmin = INT32_MIN;
      }
      if ((Devices.drivemode[id] & 0x4) != 0) {
        int32_t pos = max (min (ang2pos (angle, &ModelInfoList[idx]), pmax), pmin);
        int32_t profileacc = abs (max (min (sec * 1000.0, 32767), 0));
        switch (ModelInfoList[idx].devtype) {
          case devtX: {
              struct {
                int32_t ProfileVelocity;
                int32_t GoalPosition;
              } __attribute__ ((__packed__)) block;
              block.ProfileVelocity = profileacc;
              block.GoalPosition = pos;
              return WriteBlockData (id, ADDRESS_X_PROF_VELOCITY, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
            }
            break;
          case devtPROP: {
              struct {
                int32_t ProfileVelocity;
                int32_t GoalPosition;
              } __attribute__ ((__packed__)) block;
              block.ProfileVelocity = profileacc;
              block.GoalPosition = pos;
              return WriteBlockData (id, ADDRESS_PROP_PROF_VELOCITY, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
            }
            break;
          case devtY: {
              struct {
                int32_t ProfileVelocity;
                int32_t GoalPosition;
              } __attribute__ ((__packed__)) block;
              block.ProfileVelocity = profileacc;
              block.GoalPosition = pos;
              return WriteBlockData (id, 642, (uint8_t *)&block, 8, NULL/*&Devices.err[id]*/);
            }
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

bool DX2LIB::SetGoalAnglesAndTime2 (const uint8_t *ids, const double *angles, int num, double sec) {
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkX;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalPosition;
    uint32_t  GoalVelocity;
  } __attribute__ ((__packed__)) TBulkPRO;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkPROP;
  bool result = false;
  uint8_t *data = (uint8_t *)malloc (sizeof (TBulkPRO) * num);
  int bytes = 0;
  initarray();
  if (ids != NULL && angles != NULL) {
    for (int i = 0; i < num; i++) {
      uint8_t id = ids[i];
      if (id <= 252) {
        int idx = Devices.id2model[id];
        if (idx >= 2) {
          int32_t pmax, pmin;
          if (Devices.opmode[id] == 3) {
            pmax = ModelInfoList[idx].positionlimit.max;
            pmin = ModelInfoList[idx].positionlimit.min;
          } else {
            pmax = INT32_MAX;
            pmin = INT32_MIN;
          }
          if ((Devices.drivemode[id] & 0x4) != 0) {
            int32_t pos = max (min (ang2pos (angles[i], &ModelInfoList[idx]), pmax), pmin);
            int32_t profileacc = abs (max (min (sec * 1000.0, 32767), 0));
            switch (ModelInfoList[idx].devtype) {
              case devtX:
                ((TBulkX *) (&data[bytes]))->id = id;
                ((TBulkX *) (&data[bytes]))->addr = ADDRESS_X_PROF_VELOCITY;
                ((TBulkX *) (&data[bytes]))->size = 8;
                ((TBulkX *) (&data[bytes]))->ProfileVelocity = profileacc;
                ((TBulkX *) (&data[bytes]))->GoalPosition = pos;
                bytes += sizeof (TBulkX);
                break;
              case devtPROP:
                ((TBulkPROP *) (&data[bytes]))->id = id;
                ((TBulkPROP *) (&data[bytes]))->addr = ADDRESS_PROP_PROF_VELOCITY;
                ((TBulkPROP *) (&data[bytes]))->size = 8;
                ((TBulkPROP *) (&data[bytes]))->ProfileVelocity = profileacc;
                ((TBulkPROP *) (&data[bytes]))->GoalPosition = pos;
                bytes += sizeof (TBulkPROP);
                break;
              case devtY:
                ((TBulkPROP *) (&data[bytes]))->id = id;
                ((TBulkPROP *) (&data[bytes]))->addr = 642;
                ((TBulkPROP *) (&data[bytes]))->size = 8;
                ((TBulkPROP *) (&data[bytes]))->ProfileVelocity = profileacc;
                ((TBulkPROP *) (&data[bytes]))->GoalPosition = pos;
                bytes += sizeof (TBulkPROP);
                break;
              default:
                break;
            }
          } else break;
        }
      }
    }
    if (bytes > 0) result = WriteBulkData (data, bytes);
  }
  free (data);
  return result;
}

//-------------------------------------------------
// Set current
//-------------------------------------------------
bool DX2LIB::SetGoalCurrent (uint8_t id, double current) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      switch (ModelInfoList[idx].devtype) {
        case devtX: {
            int16_t cur = current / ModelInfoList[idx].currentratio;
            return WriteWordData (id, ADDRESS_X_GOAL_CURRENT, cur, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtPRO: {
            int16_t cur = current / ModelInfoList[idx].currentratio;
            return WriteLongData (id, ADDRESS_PRO_GOAL_TORQUE, cur, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtPROP: {
            int16_t cur = current / ModelInfoList[idx].currentratio;
            return WriteLongData (id, ADDRESS_PROP_GOAL_CURRENT, cur, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtY: {
            int16_t cur = current / ModelInfoList[idx].currentratio;
            return WriteLongData (id, ADDRESS_Y_GOAL_CURRENT, cur, NULL/*&Devices.err[id]*/);
          }
          break;
        default:
          break;
      }
    }
  }
  return false;
}

bool DX2LIB::SetGoalCurrents (const uint8_t *ids, const double *currents, int num) {
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    int16_t   GoalCurrent;
  } __attribute__ ((__packed__)) TBulk;
  bool result = false;
  uint8_t *data = (uint8_t *)malloc (sizeof (TBulk) * num);
  int bytes = 0;
  initarray();
  if (ids != NULL && currents != NULL) {
    for (int i = 0; i < num; i++) {
      uint8_t id = ids[i];
      if (id <= 252) {
        int idx = Devices.id2model[id];
        if (idx >= 2) {
          int16_t cur = currents[i] / ModelInfoList[idx].currentratio;
          switch (ModelInfoList[idx].devtype) {
            case devtX:
              ((TBulk *) (&data[bytes]))->id = id;
              ((TBulk *) (&data[bytes]))->addr = ADDRESS_X_GOAL_CURRENT;
              ((TBulk *) (&data[bytes]))->size = 2;
              ((TBulk *) (&data[bytes]))->GoalCurrent = cur;
              bytes += sizeof (TBulk);
              break;
            case devtPRO:
              ((TBulk *) (&data[bytes]))->id = id;
              ((TBulk *) (&data[bytes]))->addr = ADDRESS_PRO_GOAL_TORQUE;
              ((TBulk *) (&data[bytes]))->size = 2;
              ((TBulk *) (&data[bytes]))->GoalCurrent = cur;
              bytes += sizeof (TBulk);
              break;
            case devtPROP:
              ((TBulk *) (&data[bytes]))->id = id;
              ((TBulk *) (&data[bytes]))->addr = ADDRESS_PROP_GOAL_CURRENT;
              ((TBulk *) (&data[bytes]))->size = 2;
              ((TBulk *) (&data[bytes]))->GoalCurrent = cur;
              bytes += sizeof (TBulk);
              break;
            case devtY:
              ((TBulk *) (&data[bytes]))->id = id;
              ((TBulk *) (&data[bytes]))->addr = ADDRESS_Y_GOAL_CURRENT;
              ((TBulk *) (&data[bytes]))->size = 2;
              ((TBulk *) (&data[bytes]))->GoalCurrent = cur;
               bytes += sizeof (TBulk);
              break;
            default:
              break;
          }
        }
      }
    }
    if (bytes > 0) result = WriteBulkData (data, bytes);
  }
  free (data);
  return result;
}

//-------------------------------------------------
// Get current
//-------------------------------------------------
bool DX2LIB::GetPresentCurrent (uint8_t id, double *current) {
  initarray();
  if (current != NULL) {
    if (id <= 252) {
      int idx = Devices.id2model[id];
      if (idx >= 2) {
        int16_t cur;
        switch (ModelInfoList[idx].devtype) {
          case devtXL320:
            if (ReadWordData (id, 41, (uint16_t *)&cur, NULL/*&Devices.err[id]*/)) {
              if (cur & 0x400) cur = - (cur & 0x3ff);
              *current = (double)cur * ModelInfoList[idx].currentratio;
              return true;
            }
            break;
          case devtX:
            if (ReadWordData (id, ADDRESS_X_PRESENT_CURRENT, (uint16_t *)&cur, NULL/*&Devices.err[id]*/)) {
              *current = (double)cur * ModelInfoList[idx].currentratio;
              return true;
            }
            break;
          case devtPRO:
            if (ReadWordData (id, ADDRESS_PRO_PRESENT_CURRENT, (uint16_t *)&cur, NULL/*&Devices.err[id]*/)) {
              *current = (double)cur * ModelInfoList[idx].currentratio;
              return true;
            }
            break;
          case devtPROP:
            if (ReadWordData (id, ADDRESS_PROP_PRESENT_CURRENT, (uint16_t *)&cur, NULL/*&Devices.err[id]*/)) {
              *current = (double)cur * ModelInfoList[idx].currentratio;
              return true;
            }
            break;
          case devtY:
            if (ReadWordData (id, ADDRESS_Y_PRESENT_CURRENT, (uint16_t *)&cur, NULL/*&Devices.err[id]*/)) {
              *current = (double)cur * ModelInfoList[idx].currentratio;
              return true;
            }
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

bool DX2LIB::GetPresentCurrents (const uint8_t *ids, double *currents, int num) {
  bool result = true;
  initarray();
  if (ids != NULL && currents != NULL) {
    for (int i = 0; i < num; i++) {
      int idx = Devices.id2model[ids[i]];
      if (idx >= 2) {
        if (!GetPresentCurrent (ids[i], &currents[i])) result = false;
      }
    }
  }
  return result;
}

//-------------------------------------------------
// Set PWM
//-------------------------------------------------
bool DX2LIB::SetGoalPWM (uint8_t id, double pwm) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      switch (ModelInfoList[idx].devtype) {
        case devtX: {
            int32_t p = max (min (pwm / ModelInfoList[idx].pwmratio, ModelInfoList[idx].pwmlimit.max), ModelInfoList[idx].pwmlimit.min);
            return WriteWordData (id, ADDRESS_X_GOAL_PWM, p, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtPROP: {
            int32_t p = max (min (pwm / ModelInfoList[idx].pwmratio, ModelInfoList[idx].pwmlimit.max), ModelInfoList[idx].pwmlimit.min);
            return WriteWordData (id, ADDRESS_PROP_GOAL_PWM, p, NULL/*&Devices.err[id]*/);
          }
          break;
        case devtY: {
            int32_t p = max (min (pwm / ModelInfoList[idx].pwmratio, ModelInfoList[idx].pwmlimit.max), ModelInfoList[idx].pwmlimit.min);
            return WriteWordData (id, ADDRESS_Y_GOAL_PWM, p, NULL/*&Devices.err[id]*/);
          }
          break;
        default:
          break;
      }
    }
  }
  return false;
}

bool DX2LIB::SetGoalPWMs (const uint8_t *ids, const double *pwms, int num) {
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    int16_t   GoalPWM;
  } __attribute__ ((__packed__)) TBulk;
  bool result = false;
  uint8_t *data = (uint8_t *)malloc (sizeof (TBulk) * num);
  int bytes = 0;
  initarray();
  if (ids != NULL && pwms != NULL) {
    for (int i = 0; i < num; i++) {
      uint8_t id = ids[i];
      if (id <= 252) {
        int idx = Devices.id2model[id];
        if (idx >= 2) {
          int32_t pwm = max (min (pwms[i] / ModelInfoList[idx].pwmratio, ModelInfoList[idx].pwmlimit.max), ModelInfoList[idx].pwmlimit.min);
          switch (ModelInfoList[idx].devtype) {
            case devtX:
              ((TBulk *) (&data[bytes]))->id = id;
              ((TBulk *) (&data[bytes]))->addr = ADDRESS_X_GOAL_PWM;
              ((TBulk *) (&data[bytes]))->size = 2;
              ((TBulk *) (&data[bytes]))->GoalPWM = pwm;
              bytes += sizeof (TBulk);
              break;
            case devtPROP:
              ((TBulk *) (&data[bytes]))->id = id;
              ((TBulk *) (&data[bytes]))->addr = ADDRESS_PROP_GOAL_PWM;
              ((TBulk *) (&data[bytes]))->size = 2;
              ((TBulk *) (&data[bytes]))->GoalPWM = pwm;
              bytes += sizeof (TBulk);
              break;
            case devtY:
              ((TBulk *) (&data[bytes]))->id = id;
              ((TBulk *) (&data[bytes]))->addr = ADDRESS_Y_GOAL_PWM;
              ((TBulk *) (&data[bytes]))->size = 2;
              ((TBulk *) (&data[bytes]))->GoalPWM = pwm;
              bytes += sizeof (TBulk);
              break;
            default:
              break;
          }
        }
      }
    }
    if (bytes > 0) result = WriteBulkData (data, bytes);
  }
  free (data);
  return result;
}

//-------------------------------------------------
// Get current PWM
//-------------------------------------------------
bool DX2LIB::GetPresentPWM (uint8_t id, double *pwm) {
  initarray();
  if (pwm != NULL) {
    if (id <= 252) {
      int idx = Devices.id2model[id];
      if (idx >= 2) {
        int16_t p;
        switch (ModelInfoList[idx].devtype) {
          case devtX:
            if (ReadWordData (id, ADDRESS_X_PRESENT_PWM, (uint16_t *)&p, NULL/*&Devices.err[id]*/)) {
              *pwm = (double)p * ModelInfoList[idx].pwmratio;
              return true;
            }
            break;
          case devtPROP:
            if (ReadWordData (id, ADDRESS_PROP_PRESENT_PWM, (uint16_t *)&p, NULL/*&Devices.err[id]*/)) {
              *pwm = (double)p * ModelInfoList[idx].pwmratio;
              return true;
            }
            break;
          case devtY:
            if (ReadWordData (id, ADDRESS_Y_PRESENT_PWM, (uint16_t *)&p, NULL/*&Devices.err[id]*/)) {
              *pwm = (double)p * ModelInfoList[idx].pwmratio;
              return true;
            }
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

bool DX2LIB::GetPresentPWMs (const uint8_t *ids, double *pwms, int num) {
  bool result = true;
  initarray();
  if (ids != NULL && pwms != NULL) {
    for (int i = 0; i < num; i++) {
      int idx = Devices.id2model[ids[i]];
      if (idx >= 2) {
        if (!GetPresentPWM (ids[i], &pwms[i])) result = false;
      }
    }
  }
  return result;
}

//-------------------------------------------------
// Set Multiple
//-------------------------------------------------
bool DX2LIB::SetMultipleGoalValues (const uint8_t *ids, const double *values, int num, double sec) {
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    int16_t   GoalCurrent;
  } __attribute__ ((__packed__)) TBulk_Cur;

  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint16_t  MovingSpeed;
  } __attribute__ ((__packed__)) TBulkXL320_Velo;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalVelocity;
  } __attribute__ ((__packed__)) TBulkX_Velo;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalVelocity;
  } __attribute__ ((__packed__)) TBulkPRO_Velo;

  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalVelocity;
    uint32_t  ProfileAcceleration;
  } __attribute__ ((__packed__)) TBulkX_VeloProfAcc;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalVelocity;
    uint32_t  ProfileAcceleration;
  } __attribute__ ((__packed__)) TBulkPRO_VeloProfAcc;

  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint16_t  GoalPosition;
    uint16_t  MovingSpeed;
  } __attribute__ ((__packed__)) TBulkXL320_PosVelo;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkX_PosVelo;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  GoalPosition;
    uint32_t  GoalVelocity;
  } __attribute__ ((__packed__)) TBulkPRO_PosVelo;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkPROP_PosVelo;

  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkX_PosProfVelo;
  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    uint32_t  ProfileVelocity;
    uint32_t  GoalPosition;
  } __attribute__ ((__packed__)) TBulkPROP_PosProfVelo;

  typedef struct {
    uint8_t   id;
    uint16_t  addr;
    uint16_t  size;
    int16_t   GoalPWM;
  } __attribute__ ((__packed__)) TBulk_PWM;

  bool result = false;
  uint8_t *data = (uint8_t *)malloc (sizeof (TBulkPROP_PosProfVelo) * num);
  int bytes = 0;
  initarray();
  if (ids != NULL && values != NULL) {
    for (int i = 0; i < num; i++) {
      uint8_t id = ids[i];
      if (id <= 252) {
        int idx = Devices.id2model[id];
        if (idx >= 2) {
          switch (Devices.opmode[id]) {
            case 0: {   // current
                int16_t cur = values[i] / ModelInfoList[idx].currentratio;
                switch (ModelInfoList[idx].devtype) {
                  case devtX:
                    ((TBulk_Cur *) (&data[bytes]))->id = id;
                    ((TBulk_Cur *) (&data[bytes]))->addr = ADDRESS_X_GOAL_CURRENT;
                    ((TBulk_Cur *) (&data[bytes]))->size = 2;
                    ((TBulk_Cur *) (&data[bytes]))->GoalCurrent = cur;
                    bytes += sizeof (TBulk_Cur);
                    break;
                  case devtPRO:
                    ((TBulk_Cur *) (&data[bytes]))->id = id;
                    ((TBulk_Cur *) (&data[bytes]))->addr = ADDRESS_PRO_GOAL_TORQUE;
                    ((TBulk_Cur *) (&data[bytes]))->size = 2;
                    ((TBulk_Cur *) (&data[bytes]))->GoalCurrent = cur;
                    bytes += sizeof (TBulk_Cur);
                    break;
                  case devtPROP:
                    ((TBulk_Cur *) (&data[bytes]))->id = id;
                    ((TBulk_Cur *) (&data[bytes]))->addr = ADDRESS_PROP_GOAL_CURRENT;
                    ((TBulk_Cur *) (&data[bytes]))->size = 2;
                    ((TBulk_Cur *) (&data[bytes]))->GoalCurrent = cur;
                    bytes += sizeof (TBulk_Cur);
                    break;
                  case devtY:
                    ((TBulk_Cur *) (&data[bytes]))->id = id;
                    ((TBulk_Cur *) (&data[bytes]))->addr = ADDRESS_Y_GOAL_CURRENT;
                    ((TBulk_Cur *) (&data[bytes]))->size = 2;
                    ((TBulk_Cur *) (&data[bytes]))->GoalCurrent = cur;
                    bytes += sizeof (TBulk_Cur);
                    break;
                  default:
                    break;
                }
              }
              break;
            case 1: {   // velocity
                int32_t velo = max (min (values[i] / ModelInfoList[idx].velocityratio, ModelInfoList[idx].velocitylimit.max), ModelInfoList[idx].velocitylimit.min);
                int32_t prevvelo, profileacc = abs (max (min (sec * 1000.0, 32767), 0));
                switch (ModelInfoList[idx].devtype) {
                  case devtXL320:
                    ((TBulkXL320_Velo *) (&data[bytes]))->id = id;
                    ((TBulkXL320_Velo *) (&data[bytes]))->addr = 32;
                    ((TBulkXL320_Velo *) (&data[bytes]))->size = 2;
                    if (velo < 0) velo = abs (velo) | 0x400;
                    ((TBulkXL320_Velo *) (&data[bytes]))->MovingSpeed = velo;
                    bytes += sizeof (TBulkXL320_Velo);
                    break;
                  case devtX:
                    if (ReadLongData (id, ADDRESS_X_GOAL_VELOCITY, (uint32_t *)&prevvelo, NULL/*&Devices.err[id]*/)) {
                      if ((velo > 0 && prevvelo < 0) || (velo < 0 && prevvelo > 0)) profileacc >>= 1;
                    }
                    ((TBulkX_VeloProfAcc *) (&data[bytes]))->id = id;
                    ((TBulkX_VeloProfAcc *) (&data[bytes]))->addr = ADDRESS_X_GOAL_VELOCITY;
                    ((TBulkX_VeloProfAcc *) (&data[bytes]))->size = 8;
                    ((TBulkX_VeloProfAcc *) (&data[bytes]))->GoalVelocity = velo;
                    ((TBulkX_VeloProfAcc *) (&data[bytes]))->ProfileAcceleration = profileacc;
                    bytes += sizeof (TBulkX_VeloProfAcc);
                    break;
                  case devtPRO:
                    ((TBulkPRO_Velo *) (&data[bytes]))->id = id;
                    ((TBulkPRO_Velo *) (&data[bytes]))->addr = ADDRESS_PRO_GOAL_VELOCITY;
                    ((TBulkPRO_Velo *) (&data[bytes]))->size = 4;
                    ((TBulkPRO_Velo *) (&data[bytes]))->GoalVelocity = velo;
                    bytes += sizeof (TBulkPRO_Velo);
                    break;
                  case devtPROP:
                    if (ReadLongData (id, ADDRESS_PROP_GOAL_VELOCITY, (uint32_t *)&prevvelo, NULL/*&Devices.err[id]*/)) {
                      if ((velo > 0 && prevvelo < 0) || (velo < 0 && prevvelo > 0)) profileacc >>= 1;
                    }
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->id = id;
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->addr = ADDRESS_PROP_GOAL_VELOCITY;
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->size = 8;
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->GoalVelocity = velo;
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->ProfileAcceleration = profileacc;
                    bytes += sizeof (TBulkPRO_VeloProfAcc);
                    break;
                  case devtY:
                    if (ReadLongData (id, ADDRESS_Y_GOAL_VELOCITY, (uint32_t *)&prevvelo, NULL/*&Devices.err[id]*/)) {
                      if ((velo > 0 && prevvelo < 0) || (velo < 0 && prevvelo > 0)) profileacc >>= 1;
                    }
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->id = id;
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->addr = ADDRESS_Y_GOAL_VELOCITY;
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->size = 8;
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->GoalVelocity = velo;
                    ((TBulkPRO_VeloProfAcc *) (&data[bytes]))->ProfileAcceleration = profileacc;
                    bytes += sizeof (TBulkPRO_VeloProfAcc);
                    break;
                  default:
                    break;
                }
              }
              break;
            case 3 ... 5: {   // pos + profile velo
                int32_t pmax, pmin;
                if (Devices.opmode[id] == 3) {
                  pmax = ModelInfoList[idx].positionlimit.max;
                  pmin = ModelInfoList[idx].positionlimit.min;
                } else {
                  pmax = INT32_MAX;
                  pmin = INT32_MIN;
                }
                // Velocity-based Profile
                if ((Devices.drivemode[id] & 0x4) == 0) {
                  double pangle;
                  if (GetPresentAngle (id, &pangle)) {
                    double velocity = fabs ((pangle - values[i]) / sec);
                    int32_t pos = max (min (ang2pos (values[i], &ModelInfoList[idx]), pmax), pmin);
                    int32_t velo = abs (max (min (velocity / ModelInfoList[idx].velocityratio, ModelInfoList[idx].velocitylimit.max), ModelInfoList[idx].velocitylimit.min));
                    switch (ModelInfoList[idx].devtype) {
                      case devtXL320:
                        ((TBulkXL320_PosVelo *) (&data[bytes]))->id = id;
                        ((TBulkXL320_PosVelo *) (&data[bytes]))->addr = 30;
                        ((TBulkXL320_PosVelo *) (&data[bytes]))->size = 4;
                        ((TBulkXL320_PosVelo *) (&data[bytes]))->GoalPosition = pos;
                        if (velo < 0) velo = velo | 0x400;
                        ((TBulkXL320_PosVelo *) (&data[bytes]))->MovingSpeed = velo;
                        bytes += sizeof (TBulkXL320_PosVelo);
                        break;
                      case devtX:
                        ((TBulkX_PosVelo *) (&data[bytes]))->id = id;
                        ((TBulkX_PosVelo *) (&data[bytes]))->addr = ADDRESS_X_PROF_VELOCITY;
                        ((TBulkX_PosVelo *) (&data[bytes]))->size = 8;
                        ((TBulkX_PosVelo *) (&data[bytes]))->ProfileVelocity = velo;
                        ((TBulkX_PosVelo *) (&data[bytes]))->GoalPosition = pos;
                        bytes += sizeof (TBulkX_PosVelo);
                        break;
                      case devtPRO:
                        ((TBulkPRO_PosVelo *) (&data[bytes]))->id = id;
                        ((TBulkPRO_PosVelo *) (&data[bytes]))->addr = ADDRESS_PRO_GOAL_POSITION;
                        ((TBulkPRO_PosVelo *) (&data[bytes]))->size = 8;
                        ((TBulkPRO_PosVelo *) (&data[bytes]))->GoalPosition = pos;
                        ((TBulkPRO_PosVelo *) (&data[bytes]))->GoalVelocity = velo;
                        bytes += sizeof (TBulkPRO_PosVelo);
                        break;
                      case devtPROP:
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->id = id;
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->addr = ADDRESS_PROP_PROF_VELOCITY;
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->size = 8;
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->ProfileVelocity = velo;
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->GoalPosition = pos;
                        bytes += sizeof (TBulkPROP_PosVelo);
                        break;
                      case devtY:
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->id = id;
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->addr = 634;
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->size = 8;
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->ProfileVelocity = velo;
                        ((TBulkPROP_PosVelo *) (&data[bytes]))->GoalPosition = pos;
                        break;
                      default:
                        break;
                    }
                  }
                // Time-based Profile
                } else {
                  int32_t pos = max (min (ang2pos (values[i], &ModelInfoList[idx]), pmax), pmin);
                  int32_t profilevelo = abs (max (min (sec * 1000.0, 32767), 0));
                  switch (ModelInfoList[idx].devtype) {
                    case devtX:
                      ((TBulkX_PosProfVelo *) (&data[bytes]))->id = id;
                      ((TBulkX_PosProfVelo *) (&data[bytes]))->addr = ADDRESS_X_PROF_VELOCITY;
                      ((TBulkX_PosProfVelo *) (&data[bytes]))->size = 8;
                      ((TBulkX_PosProfVelo *) (&data[bytes]))->ProfileVelocity = profilevelo;
                      ((TBulkX_PosProfVelo *) (&data[bytes]))->GoalPosition = pos;
                      bytes += sizeof (TBulkX_PosProfVelo);
                      break;
                    case devtPROP:
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->id = id;
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->addr = ADDRESS_PROP_PROF_VELOCITY;
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->size = 8;
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->ProfileVelocity = profilevelo;
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->GoalPosition = pos;
                      bytes += sizeof (TBulkPROP_PosProfVelo);
                      break;
                    case devtY:
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->id = id;
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->addr = 642;
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->size = 8;
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->ProfileVelocity = profilevelo;
                      ((TBulkPROP_PosProfVelo *) (&data[bytes]))->GoalPosition = pos;
                      bytes += sizeof (TBulkPROP_PosProfVelo);
                      break;
                    default:
                      break;
                  }
                }
              }
              break;
            case 16: {  // PWM
                int32_t pwm = max (min (values[i] / ModelInfoList[idx].pwmratio, ModelInfoList[idx].pwmlimit.max), ModelInfoList[idx].pwmlimit.min);
                switch (ModelInfoList[idx].devtype) {
                  case devtX:
                    ((TBulk_PWM *) (&data[bytes]))->id = id;
                    ((TBulk_PWM *) (&data[bytes]))->addr = ADDRESS_X_GOAL_PWM;
                    ((TBulk_PWM *) (&data[bytes]))->size = 2;
                    ((TBulk_PWM *) (&data[bytes]))->GoalPWM = pwm;
                    bytes += sizeof (TBulk_PWM);
                    break;
                  case devtPROP:
                    ((TBulk_PWM *) (&data[bytes]))->id = id;
                    ((TBulk_PWM *) (&data[bytes]))->addr = ADDRESS_PROP_GOAL_PWM;
                    ((TBulk_PWM *) (&data[bytes]))->size = 2;
                    ((TBulk_PWM *) (&data[bytes]))->GoalPWM = pwm;
                    bytes += sizeof (TBulk_PWM);
                    break;
                  case devtY:
                    ((TBulk_PWM *) (&data[bytes]))->id = id;
                    ((TBulk_PWM *) (&data[bytes]))->addr = ADDRESS_Y_GOAL_PWM;
                    ((TBulk_PWM *) (&data[bytes]))->size = 2;
                    ((TBulk_PWM *) (&data[bytes]))->GoalPWM = pwm;
                    bytes += sizeof (TBulk_PWM);
                    break;
                  default:
                    break;
                }
              }
              break;

          }
        }
      }
    }
    if (bytes > 0) result = WriteBulkData (data, bytes);
  }
  free (data);
  return result;
}


//-------------------------------------------------
// Change drive mode
//-------------------------------------------------
/*
   The value of mode depends on the model (the following is for the X series)
    bit3: Torque On by Goal Update
    bit2: Profile Config (0:Velocity-based, 1:Time-based)
    bit1: Master/Slave Mode (0:Master, 1:Slave)
    bit0: Normal/Reverse Mode (0:CCW Positive, 1:CW Positive)
 */
bool DX2LIB::SetDriveMode (uint8_t id, uint8_t mode) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      uint8_t rmode;
      if (GetOperatingMode (id, &rmode)) {
        if (mode == Devices.drivemode[id]) {
          return true;
        } else {
          if (SetTorqueEnable (id, false)) {
            switch (ModelInfoList[idx].devtype) {
              case devtX:
              case devtPRO:
                if (WriteByteData (id, ADDRESS_X_DRIVE_MODE, mode, NULL/*&Devices.err[id]*/)) {
                  if (GetOperatingMode (id, &rmode)) {
                    return (mode == Devices.drivemode[id]);
                  }
                }
                break;
              case devtPROP:
                if (WriteByteData (id, ADDRESS_PROP_DRIVE_MODE, mode, NULL/*&Devices.err[id]*/)) {
                  if (GetOperatingMode (id, &rmode)) {
                    return (mode == Devices.drivemode[id]);
                  }
                }
                break;
              case devtY:
                if (WriteByteData (id, ADDRESS_Y_DRIVE_MODE, mode, NULL/*&Devices.err[id]*/)) {
                  if (GetOperatingMode (id, &rmode)) {
                    return (mode == Devices.drivemode[id]);
                  }
                }
                break;
              default:
                break;
            }
          }
        }
      }
    }
  }
  return false;
}

bool DX2LIB::SetDriveModesEquival (const uint8_t *ids, int num, uint8_t mode) {
  bool result = true;
  initarray();
  if (ids != NULL) {
    for (int i = 0; i < num; i++) {
      int idx = Devices.id2model[ids[i]];
      if (idx >= 2) {
        if (!SetDriveMode (ids[i], mode)) result = false;
      }
    }
  }
  return result;
}

//-------------------------------------------------
// Set operating mode
//-------------------------------------------------
/*
   mode values are based on X series
    0: cur
    1: velo
    3: pos
    4: expos(pos+multiturn)
    5: expos(pos+multiturn+cur)
    16:PWM
 */
#include <stdio.h>
bool DX2LIB::SetOperatingMode (uint8_t id, uint8_t mode) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) {
      uint8_t rmode = 255;
      if (GetOperatingMode (id, &rmode)) {
        if (mode == rmode) {
          return true;
        } else {
          if (SetTorqueEnable (id, false)) {
            switch (ModelInfoList[idx].devtype) {
              case devtXL320:
                switch (mode) {
                  case 1: // velo
                    if (WriteByteData (id, ADDRESS_X_OPERATING_MODE, 1, NULL/*&Devices.err[id]*/)) {
                      if (GetOperatingMode (id, &rmode)) {
                        return (mode == rmode);
                      }
                    }
                    break;
                  case 3: // pos
                    if (WriteByteData (id, ADDRESS_X_OPERATING_MODE, 2, NULL/*&Devices.err[id]*/)) {
                      if (GetOperatingMode (id, &rmode)) {
                        return (mode == rmode);
                      }
                    }
                    break;
                }
                break;
              case devtX:
              case devtPRO:
              case devtPROP:
                if (WriteByteData (id, ADDRESS_X_OPERATING_MODE, mode, NULL/*&Devices.err[id]*/)) {
                  if (GetOperatingMode (id, &rmode)) {
                    return (mode == rmode);
                  }
                }
                break;
              case devtY:
                if (WriteByteData (id, ADDRESS_Y_OPERATING_MODE, mode, NULL/*&Devices.err[id]*/)) {
                  if (GetOperatingMode (id, &rmode)) {
                    return (mode == rmode);
                  }
                }
                break;
              default:
                break;
            }
          }
        }
      }
    }
  }
  return false;
}

bool DX2LIB::SetOperatingModesEquival (const uint8_t *ids, int num, uint8_t mode) {
  bool result = true;
  initarray();
  if (ids != NULL) {
    for (int i = 0; i < num; i++) {
      int idx = Devices.id2model[ids[i]];
      if (idx >= 2) {
        if (!SetOperatingMode (ids[i], mode)) result = false;
      }
    }
  }
  return result;
}

//-------------------------------------------------
// Get operating mode
//-------------------------------------------------
/*
   mode values are based on X series
    0: cur
    1: velo
    3: pos
    4: expos(pos+multiturn)
    5: expos(pos+multiturn+cur)
    16:PWM
 */
bool DX2LIB::GetOperatingMode (uint8_t id, uint8_t *mode) {
  initarray();
  if (mode != NULL) {
    if (id <= 252) {
      int idx = Devices.id2model[id];
      uint8_t rmode = 255, rdmode = 0;
      if (idx >= 2) {
        switch (ModelInfoList[idx].devtype) {
          case devtXL320:
            if (ReadByteData (id, 11, &rmode, NULL/*&Devices.err[id]*/)) {
              switch (rmode) {
                case 1:
                  Devices.opmode[id] = (*mode = 1);
                  return true;
                  break;
                case 2:
                  Devices.opmode[id] = (*mode = 3);
                  return true;
                  break;
              }
            }
            break;
          case devtX:
            if (ReadByteData (id, ADDRESS_X_OPERATING_MODE, &rmode, NULL/*&Devices.err[id]*/) && ReadByteData (id, 10, &rdmode, NULL/*&Devices.err[id]*/)) {
              Devices.opmode[id] = (*mode = rmode);
              Devices.drivemode[id] = rdmode;
              return true;
            }
            break;
          case devtPRO:
            if (ReadByteData (id, ADDRESS_PRO_OPERATING_MODE, &rmode, NULL/*&Devices.err[id]*/)) {
              Devices.opmode[id] = (*mode = rmode);
              return true;
            }
            break;
          case devtPROP:
            if (ReadByteData (id, ADDRESS_PROP_OPERATING_MODE, &rmode, NULL/*&Devices.err[id]*/)) {
              Devices.opmode[id] = (*mode = rmode);
              return true;
            }
            break;
          case devtY:
            if (ReadByteData (id, ADDRESS_Y_OPERATING_MODE, &rmode, NULL/*&Devices.err[id]*/)) {
              Devices.opmode[id] = (*mode = rmode);
              return true;
            }
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

//-------------------------------------------------
// Get hardware error for specified ID
//-------------------------------------------------
bool DX2LIB::GetHWErrorCode (uint8_t id, uint8_t *hwerr) {
  initarray();
  if (hwerr != NULL) {
    if (id <= 252) {
      int idx = Devices.id2model[id];
      if (idx >= 2) {
        switch (ModelInfoList[idx].devtype) {
          case devtXL320:
            return ReadByteData (id, 50, hwerr, NULL/*&Devices.err[id]*/);
            break;
          case devtX:
            return ReadByteData (id, ADDRESS_X_HARDWARE_ERROR_STATUS, hwerr, NULL/*&Devices.err[id]*/);
            break;
          case devtPRO:
            return ReadByteData (id, ADDRESS_PRO_HARDWARE_ERROR_STATUS, hwerr, NULL/*&Devices.err[id]*/);
            break;
          case devtPROP:
            return ReadByteData (id, ADDRESS_PROP_HARDWARE_ERROR_STATUS, hwerr, NULL/*&Devices.err[id]*/);
            break;
          case devtY:
            return ReadByteData (id, ADDRESS_Y_ERROR_CODE, hwerr, NULL/*&Devices.err[id]*/);
            break;
          default:
            break;
        }
      }
    }
  }
  return false;
}

//-------------------------------------------------
// Get the recorded error code for the specified ID
//-------------------------------------------------
TErrorCode DX2LIB::GetErrorCode (uint8_t id) {
  initarray();
  if (id <= 252) {
    int idx = Devices.id2model[id];
    if (idx >= 2) return /*Devices.err[id]*/0;
  }
  return 0xffff;
}

//-------------------------------------------------
// Get devise information for the specified ID
//-------------------------------------------------
PDXL_ModelInfo DX2LIB::GetModelInfo (uint8_t id) {
  uint16_t    model;
  uint8_t     mode, rdelay;
  uint16_t    indirect[16];

  initarray();
  if (ReadWordData (id, 0, &model, NULL)) {
    for (unsigned int i = 2; i < (sizeof (ModelInfoList) / sizeof (ModelInfoList[0])); i++) {
      if (ModelInfoList[i].modelno == model) {
        // デバイスのReturn Statusを2に、Return Delay Timeを0に変更
        // YシリーズのみIndirectを使ってProfileとGoalを隣接させる
        if (Devices.id2model[id] == 0) {
          switch (ModelInfoList[i].devtype) {
            case devtXL320:
              SetTorqueEnable (id, false);
              WriteByteData (id, 17, 2, NULL/*&Devices.err[id]*/);
              if (ReadByteData (id, 5, &rdelay, NULL/*&Devices.err[id]*/)) {
                if (rdelay != 0) {
                  WriteByteData (id, 5, 0, NULL/*&Devices.err[id]*/);
                }
              }
              break;
            case devtX:
              SetTorqueEnable (id, false);
              WriteByteData (id, ADDRESS_X_STATUS_RETURN_LEVEL, 2, NULL/*&Devices.err[id]*/);
              if (ReadByteData (id, ADDRESS_X_RETURN_DELAY_TIME, &rdelay, NULL/*&Devices.err[id]*/)) {
                if (rdelay != 0) {
                  WriteByteData (id, ADDRESS_X_RETURN_DELAY_TIME, 0, NULL/*&Devices.err[id]*/);
                }
              }
              break;
            case devtPRO:
              SetTorqueEnable (id, false);
              WriteByteData (id, ADDRESS_PRO_STATUS_RETURN_LEVEL, 2, NULL/*&Devices.err[id]*/);
              if (ReadByteData (id, ADDRESS_X_RETURN_DELAY_TIME, &rdelay, NULL/*&Devices.err[id]*/)) {
                if (rdelay != 0) {
                  WriteByteData (id, ADDRESS_PRO_RETURN_DELAY_TIME, 0, NULL/*&Devices.err[id]*/);
                }
              }
              break;
            case devtPROP:
              SetTorqueEnable (id, false);
              WriteByteData (id, ADDRESS_PROP_STATUS_RETURN_LEVEL, 2, NULL/*&Devices.err[id]*/);
              if (ReadByteData (id, ADDRESS_PROP_RETURN_DELAY_TIME, &rdelay, NULL/*&Devices.err[id]*/)) {
                if (rdelay != 0) {
                  WriteByteData (id, ADDRESS_PROP_RETURN_DELAY_TIME, 0, NULL/*&Devices.err[id]*/);
                }
              }
              break;
            case devtY:
              SetTorqueEnable (id, false);
              WriteByteData (id, ADDRESS_Y_STATUS_RETURN_LEVEL, 2, NULL/*&Devices.err[id]*/);
              if (ReadByteData (id, ADDRESS_Y_RETURN_DELAY_TIME, &rdelay, NULL/*&Devices.err[id]*/)) {
                if (rdelay != 0) {
                  WriteByteData (id, ADDRESS_Y_RETURN_DELAY_TIME, 0, NULL/*&Devices.err[id]*/);
                }
              }
              if (ReadBlockData (id, 256, (uint8_t *)&indirect, 8, NULL/*&Devices.err[id]*/)) {
                if ((indirect[0] != ADDRESS_Y_PROF_VELOCITY) || (indirect[4] != ADDRESS_Y_GOAL_POSITION) || (indirect[8] != ADDRESS_Y_PROF_TIME) || (indirect[12] != ADDRESS_Y_GOAL_POSITION)) {
                  indirect[0] = ADDRESS_Y_PROF_VELOCITY;
                  indirect[1] = ADDRESS_Y_PROF_VELOCITY + 1;
                  indirect[2] = ADDRESS_Y_PROF_VELOCITY + 2;
                  indirect[3] = ADDRESS_Y_PROF_VELOCITY + 3;
                  indirect[4] = ADDRESS_Y_GOAL_POSITION;
                  indirect[5] = ADDRESS_Y_GOAL_POSITION + 1;
                  indirect[6] = ADDRESS_Y_GOAL_POSITION + 2;
                  indirect[7] = ADDRESS_Y_GOAL_POSITION + 3;
                  indirect[8] = ADDRESS_Y_PROF_TIME;
                  indirect[9] = ADDRESS_Y_PROF_TIME + 1;
                  indirect[10] = ADDRESS_Y_PROF_TIME + 2;
                  indirect[11] = ADDRESS_Y_PROF_TIME + 3;
                  indirect[12] = ADDRESS_Y_GOAL_POSITION;
                  indirect[13] = ADDRESS_Y_GOAL_POSITION + 1;
                  indirect[14] = ADDRESS_Y_GOAL_POSITION + 2;
                  indirect[15] = ADDRESS_Y_GOAL_POSITION + 3;
                  WriteBlockData (id, 256, (uint8_t *)&indirect, 16, NULL/*&Devices.err[id]*/);
                }
              }
              break;
            default:
              break;
          }
        }
        Devices.id2model[id] = i;
        GetOperatingMode (id, &mode);
        return (PDXL_ModelInfo)&ModelInfoList[i];
      }
    }
    Devices.id2model[id] = 1;
    return (PDXL_ModelInfo)&ModelInfoList[1];
  }
  Devices.id2model[id] = 0;
  return (PDXL_ModelInfo)&ModelInfoList[0];
}

//-------------------------------------------------
// Configure list of connected devices
//-------------------------------------------------
static void bsort(uint8_t *ary, int sz) {
  if (sz > 0) {
    // Bubble Sort implementation
    for (int i = 0; i < sz - 1; i++) {
      for (int j = 0; j < sz - i - 1; j++) {
        if (ary[j] > ary[j + 1]) {
          // Swap values
          int tmp = ary[j];
          ary[j] = ary[j + 1];
          ary[j + 1] = tmp;
        }
      }
    }
  }
}

int DX2LIB::ScanDevices (uint8_t *ids) {
  Devices.num = 0;
  initarray();

  uint8_t n = 253;
  TAlarmStatus alm[253];
  if (Ping2 (&n, alm)) {
    for (int i = 0; i < n; i++) {
      uint8_t id = alm[i].id;
      Devices.id2model[id] = 0;
      /* Devices.err[id] = 0xffff; */
      // デバイスのReturn Delay Timeを0に変更
      switch (GetModelInfo (id)->devtype) {
        case devtXL320:
        case devtX:
        case devtPRO:
        case devtPROP:
        case devtY:
          if (ids != NULL) {
            *ids = id;
            ids++;
          }
          Devices.num++;
          break;
        default:
          break;
      }
    }
  }
  if (ids != NULL) bsort(ids, Devices.num);
  return Devices.num;
}

//-------------------------------------------------
// Displays a list of devices included in the scanned devices list
//-------------------------------------------------
bool DX2LIB::PrintDevicesList (int (*pf) (const char *, ...)) {
  bool result = false;
  initarray();
  for (int id = 0; id <= 252; id++) {
    if (Devices.id2model[id] != 0) pf ("[%3d] %-15s($%04X)\n", id, ModelInfoList[Devices.id2model[id]].name, ModelInfoList[Devices.id2model[id]].modelno);
    result = true;
  }
  return result;
}

//-------------------------------------------------
// Restore the scanned device list to its initial state
//-------------------------------------------------
void DX2LIB::InitDevicesList (void) {
  Devices.num = 0;
  for (int i = 0; i < 256; i++) {
    Devices.id2model[i] = 0;
    /* Devices.err[i] = 0xffff; */
    Devices.opmode[i] = 255;
    Devices.drivemode[i] = 255;
  }
}

//-------------------------------------------------
// Get stored device information from id
//-------------------------------------------------
PDXL_ModelInfo DX2LIB::GetStoredModelInfoByID (uint8_t id) {
  initarray ();
  return (PDXL_ModelInfo)&ModelInfoList[Devices.id2model[id]];
}

#endif

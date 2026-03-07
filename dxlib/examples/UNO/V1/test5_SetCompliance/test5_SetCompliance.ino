// Read/write items with ReadBlockData/WriteBlockData
//  Check changes in positioning control behavior depending on CMP or PID settings.
//  **Assuming DX/AX/MX Series
//  1.select [SOFT]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE at 115200bps

#include "avr_uno_softserial.h"
#include <whoareyou.h>

#define BAUDRATE  57143
#define TARGET_ID 1

DXLIB dxif((DXLIB::PDXHost_ConfParam)&param);

typedef union {
  struct {
    uint8_t CWM;
    uint8_t CCWM;
    uint8_t CWS;
    uint8_t CCWS;
  } comp;
  struct {
    uint8_t DGain;
    uint8_t IGain;
    uint8_t PGain;
  } pid;
} TCmpPid;


PModelInfo pminfo = NULL;
PMajorItemsAddr paddr = NULL;

void setup() {
  TCmpPid p;
  uint16_t modelno;

  // set terminal baudrate
  Serial.begin (115200);

  // set dxlib baudrate
  dxif.begin (BAUDRATE, 50);

  // Get previous compliance
  if (dxif.ReadWordData (TARGET_ID, 0, &modelno, NULL)) {
    pminfo = whoareyou (modelno);
    Serial.println (pminfo->name);
    if (pminfo->devtype != devtNONE) {
      paddr = itemaddr (modelno);
      if (dxif.ReadBlockData (TARGET_ID, ADDRESS_CW_COMP_MARGIN, (uint8_t *)&p, sizeof (TCmpPid), NULL)) {
        Serial.print ("Succeed to RB:");
        switch (pminfo->devtype) {
          case devtDX:
          case devtAX:
          case devtRX:
            Serial.print (p.comp.CWM);
            Serial.print (",");
            Serial.print (p.comp.CCWM);
            Serial.print (",");
            Serial.print (p.comp.CWS);
            Serial.print (",");
            Serial.println (p.comp.CCWS);
            break;
          case devtMX:
            Serial.print (p.pid.DGain);
            Serial.print (",");
            Serial.print (p.pid.IGain);
            Serial.print (",");
            Serial.println (p.pid.PGain);
            break;
        }
      } else Serial.println ("Failed to RB");
    }
  }
}

void loop() {
  const TCmpPid cmp[3] = {
    {1, 1, 2, 2},
    {1, 1, 32, 32},
    {1, 1, 64, 64}
  };
  const TCmpPid pid[3] = {
    {0, 0,  3},
    {0, 0, 10},
    {0, 0, 128}
  };
  int16_t gposs[4] = {
    (int16_t)(pminfo->positionlimit.max / 2),
    (int16_t)(pminfo->positionlimit.max / 2 - pminfo->positionlimit.max / 4),
    (int16_t)(pminfo->positionlimit.max / 2),
    (int16_t)(pminfo->positionlimit.max / 2 + pminfo->positionlimit.max / 4)
  };
  static uint16_t ind = 0;

  switch (pminfo->devtype) {
    case devtDX:
    case devtAX:
    case devtRX:
      if (dxif.WriteBlockData (TARGET_ID, ADDRESS_CW_COMP_MARGIN, (uint8_t *)&cmp[ind], sizeof (TCmpPid), NULL)) {
        Serial.print ("Comp pattern:");
        Serial.println (ind);
      }
      break;
    case devtMX:
      if (dxif.WriteBlockData (TARGET_ID, ADDRESS_CW_COMP_MARGIN, (uint8_t *)&pid[ind], sizeof (TCmpPid), NULL)) {
        Serial.print ("PID pattern:");
        Serial.println (ind);
      }
      break;
  }
  for (int i = 0; i < 4; i++) {
    dxif.WriteWordData (TARGET_ID, paddr->g_pos.addr, gposs[i], NULL);
    delay (500);
  }
  ind = (ind + 1) % 3;
}
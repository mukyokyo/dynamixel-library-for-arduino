// Set Goal Position by Sync Write
//  Set positions using WriteSyncWordData for multiple axes
//  **Assuming DX/AX/MX Series
//  1.select [SOFT]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE at 115200bps

#include <dxlib.h>
#include <dxmemmap.h>

#include "avr_uno_softserial.h"

#define BAUDRATE  57143
#define IDS_LIST  {1,2,3,4,5}

DXLIB dxif((DXLIB::PDXHost_ConfParam)&param);

const uint8_t IDs[] = IDS_LIST;

struct {
  int16_t minpos, maxpos;
} poslimit[sizeof (IDs)];

int16_t pos[sizeof (IDs)], dir[sizeof (IDs)];

void setup() {
  // set terminal baudrate
  Serial.begin (115200);

  // set dxlib baudrate
  dxif.begin (57143, 50);

  // Get max/min position limit
  for (uint8_t i = 0; i < sizeof (IDs); i++) {
    if (dxif.ReadBlockData (IDs[i], ADDRESS_CW_ANGLE_LIMIT, (uint8_t *)&poslimit[i], 4, NULL)) {
      if (dxif.ReadWordData (IDs[i], ADDRESS_PRESENT_POSITION, (uint16_t *)&pos[i], NULL)) {
        Serial.print (IDs[i]);
        Serial.print (": max pos=");
        Serial.print (poslimit[i].minpos);
        Serial.print (" min pos=");
        Serial.print (poslimit[i].maxpos);
        Serial.print (" presens pos=");
        Serial.println (pos[i]);
      }
      dir[i] = 10;
    } else
      dir[i] = 0;
  }
}

void loop() {
  // Update target value
  for (uint8_t i = 0; i < sizeof (IDs); i++) {
    pos[i] += dir[i];
    if (pos[i] >= poslimit[i].maxpos) {
      pos[i] = poslimit[i].maxpos;
      dir[i] *= -1;
    } else if (pos[i] <= poslimit[i].minpos) {
      pos[i] = poslimit[i].minpos;
      dir[i] *= -1;
    }
    Serial.print (" ");
    Serial.print (IDs[i]);
    Serial.print (":");
    Serial.print (pos[i]);
  }
  Serial.println ("");

  dxif.WriteSyncWordData (IDs, 30, (uint16_t *)pos, sizeof (IDs));

  delay (10);
}

// Set Goal Position by Sync Write
//  Set positions using WriteSyncWordData for multiple axes
//  **Assuming X?330/X?430/X?540
//  1.Switch SW1 to [SOFT]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE at 115200bps

#include "avr_uno_softserial.h"
#include <dx2memmap.h>

#define BAUDRATE 57600
#define IDS_LIST { 1, 2, 3, 4, 5 }

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

const uint8_t IDs[] = IDS_LIST;

struct {
  int32_t maxpos, minpos;
} poslimit[sizeof(IDs)];

int32_t pos[sizeof(IDs)], dir[sizeof(IDs)];

void setup() {
  // set terminal baudrate
  Serial.begin(115200);

  // set dx2lib baudrate
  dxif.begin(BAUDRATE, 50);

  // Get max/min position limit
  for (uint8_t i = 0; i < sizeof(IDs); i++) {
    if (dxif.ReadBlockData(IDs[i], ADDRESS_X_MAX_POSITION_LIMIT, (uint8_t *)&poslimit[i], 4)) {
      if (dxif.ReadLongData(IDs[i], ADDRESS_X_PRESENT_POSITION, &pos[i])) {
        Serial.print(IDs[i]);
        Serial.print(": max pos=");
        Serial.print(poslimit[i].minpos);
        Serial.print(" min pos=");
        Serial.print(poslimit[i].maxpos);
        Serial.print(" presens pos=");
        Serial.println(pos[i]);
        // Torque Enable
        dxif.WriteByteData(IDs[i], ADDRESS_X_TORQUE_ENABLE, 1);
      }
      dir[i] = 10;
    } else
      dir[i] = 0;
  }
}

void loop() {
  // Update position value
  for (uint8_t i = 0; i < sizeof(IDs); i++) {
    pos[i] += dir[i];
    if (pos[i] >= poslimit[i].maxpos) {
      pos[i] = poslimit[i].maxpos;
      dir[i] *= -1;
    } else if (pos[i] <= poslimit[i].minpos) {
      pos[i] = poslimit[i].minpos;
      dir[i] *= -1;
    }
    Serial.print(" ");
    Serial.print(IDs[i]);
    Serial.print(":");
    Serial.print(pos[i]);
  }
  Serial.println("");

  // Set goal position
  dxif.WriteSyncLongData(IDs, ADDRESS_X_GOAL_POSITION, pos, sizeof(IDs));

  delay(10);
}
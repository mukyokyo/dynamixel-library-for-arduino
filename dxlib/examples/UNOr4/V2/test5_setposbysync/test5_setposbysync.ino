// Set goal position by sync instruction
//  Set positions using WriteSyncWordData for multiple axes
//  **Assuming X?330/X?430/X?540
//  1.Switch SW1 to [HARD]
//  2.compile & download

#include "uno_r4_hardserial.h"
#include <dx2memmap.h>

#define BAUDRATE 57600
#define IDS_LIST { 1, 2, 3, 4, 5 }

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

const uint8_t IDs[] = IDS_LIST;

struct {
  int32_t maxpos, minpos;
} poslimit[sizeof(IDs)];

int32_t pos[sizeof(IDs)], dir[sizeof(IDs)];

extern "C" {
  int _write(int fd, char *ptr, int len) {
    (void)fd;
    return Serial.write(ptr, len);
  }
}

void setup() {
  Serial.begin();
  delay(2000);

  dxif.begin(BAUDRATE, 50);

  // Get max/min position limit
  for (uint8_t i = 0; i < sizeof(IDs); i++) {
    bool ok = false;
    uint8_t mode;
    if (dxif.WriteByteData(IDs[i], ADDRESS_X_TORQUE_ENABLE, 0))
      if (dxif.ReadByteData(IDs[i], ADDRESS_X_OPERATING_MODE, &mode))
        if (dxif.WriteByteData(IDs[i], ADDRESS_X_OPERATING_MODE, 3))
          if (dxif.ReadBlockData(IDs[i], ADDRESS_X_MAX_POSITION_LIMIT, (uint8_t *)&poslimit[i], 4)) {
            if (ok = (dxif.ReadLongData(IDs[i], ADDRESS_X_PRESENT_POSITION, &pos[i]))) {
              printf("%d: min=%d, max=%d, ppos=%d\n", IDs[i], poslimit[i].minpos, poslimit[i].maxpos, pos[i]);
              // Torque Enable
              dxif.WriteByteData(IDs[i], ADDRESS_X_TORQUE_ENABLE, 1);
            }
    }
    dir[i] = ok ? 10 : 0;
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
    printf("%d:%d, ", IDs[i], pos[i]);
  }
  printf("\r\n");

  // Set goal position
  dxif.WriteSyncLongData(IDs, ADDRESS_X_GOAL_POSITION, pos, sizeof(IDs));

  delay(10);
}
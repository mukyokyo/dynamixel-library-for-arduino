// Set goal position
//  Move position to the full range
//  **Assuming X?330/X?430/X?540
//  **When selecting hardware serial, baudrate must be less than 2000000bps.
//  1.Switch SW1 to [HARD]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE

//#include "avr_uno_hardserial.h"
#include "uno_r4_hardserial.h"
#include <dx2memmap.h>

#define TARGET_ID 1
#define BAUDRATE 57600

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

int32_t maxpos, minpos, gpos = 0;

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

  bool ok = false;
  uint8_t mode;
  // Torque off
  if (dxif.WriteByteData(TARGET_ID, ADDRESS_X_TORQUE_ENABLE, 0))
    // Set position ctrl mode
    if (dxif.ReadByteData(TARGET_ID, ADDRESS_X_OPERATING_MODE, &mode))
      if (dxif.WriteByteData(TARGET_ID, ADDRESS_X_OPERATING_MODE, 3))
        // Get max/min position limit
        if (dxif.ReadLongData(TARGET_ID, ADDRESS_X_MAX_POSITION_LIMIT, &maxpos))
          if (dxif.ReadLongData(TARGET_ID, ADDRESS_X_MIN_POSITION_LIMIT, &minpos))
            // Get present position
            if (dxif.ReadLongData(TARGET_ID, ADDRESS_X_PRESENT_POSITION, &gpos))
              // Torque on
              ok = dxif.WriteByteData(TARGET_ID, ADDRESS_X_TORQUE_ENABLE, 1);

  if (ok) printf("Init OK\r\n max:%d min%d ppos:%d\r\n", maxpos, minpos, gpos);
  else printf("Init Fail\r\n");
}

void loop() {
  static int32_t dir = 10;

  // Update target value
  gpos += dir;
  if (gpos >= maxpos) {
    gpos = maxpos;
    dir *= -1;
  } else if (gpos <= minpos) {
    gpos = minpos;
    dir *= -1;
  };
  // Set goal position
  dxif.WriteLongData(TARGET_ID, ADDRESS_X_GOAL_POSITION, gpos);

  delay(10);
}

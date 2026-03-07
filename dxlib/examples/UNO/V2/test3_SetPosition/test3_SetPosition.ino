// Set Goal Position
//  Move position to the full range
//  **Assuming X?330/X?430/X?540
//  **When selecting software serial, baudrate must be less than 57600bps.
//  1.Switch SW1 to [SOFT]
//  2.compile & download
//  3.If software serial, switch SW1 to [SOFT], if hardware serial, switch SW1 to [HARD].

//#include "avr_uno_hardserial.h"
#include "avr_uno_softserial.h"
#include <dx2memmap.h>

#define TARGET_ID 1
#define BAUDRATE 57600

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

int32_t maxpos, minpos, gpos = 0;

void setup() {
  dxif.begin(BAUDRATE, 50);

  // Get max/min position limit
  dxif.ReadLongData(TARGET_ID, ADDRESS_X_MAX_POSITION_LIMIT, &maxpos);
  dxif.ReadLongData(TARGET_ID, ADDRESS_X_MIN_POSITION_LIMIT, &minpos);

  // Torque on
  dxif.WriteByteData(TARGET_ID, ADDRESS_X_TORQUE_ENABLE, 1);

  // Get Present Position
  dxif.ReadLongData(TARGET_ID, ADDRESS_X_PRESENT_POSITION, &gpos);
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

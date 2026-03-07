// Get present position
//  Get present position of horn with torque off
//  **Assuming X?330/X?430/X?540
//  **When selecting hardware serial, baudrate must be less than 2000000bps.
//  1.Switch SW1 to [HARD]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE

#include "uno_r4_hardserial.h"
#include <dxmemmap.h>

#define TARGET_ID 1
#define BAUDRATE 57600

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

extern "C" {
  int _write(int fd, char *ptr, int len) {
    (void)fd;
    return Serial.write(ptr, len);
  }
}

void setup() {
  // set terminal baudrate
  Serial.begin();

  // set dxlib baudrate
  dxif.begin(BAUDRATE, 50);

  // Torque off
  dxif.WriteByteData(TARGET_ID, ADDRESS_X_TORQUE_ENABLE, 0, NULL);
}

void loop() {
  int32_t ppos;

  // Get present position
  if (dxif.ReadLongData(TARGET_ID, ADDRESS_X_PRESENT_POSITION, (uint32_t *)&ppos)) {
    printf("position=%7d\r\n", ppos);
  } else {
    printf("error\n\r");
  }

  delay(100);
}

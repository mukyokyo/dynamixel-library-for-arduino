// Get Present Position
//  Get present position of horn with torque off
//  **Assuming X?330/X?430/X?540
//  **When selecting software serial, baudrate must be less than 57600bps.
//  1.select [SOFT]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE at 115200 bps

//#include "avr_uno_hardserial.h"
#include "avr_uno_softserial.h"
#include <dxmemmap.h>

#define TARGET_ID 1
#define BAUDRATE  57600

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

void setup() {
  // set terminal baudrate
  Serial.begin (115200);

  // set dxlib baudrate
  dxif.begin (57600, 50);

  // Torque off
  dxif.WriteByteData (TARGET_ID, ADDRESS_X_TORQUE_ENABLE, 0, NULL);
}

void loop() {
  int32_t ppos;

  // Get present position
  dxif.ReadLongData (TARGET_ID, ADDRESS_X_PRESENT_POSITION, (uint32_t *)&ppos, NULL);
  Serial.println (ppos, DEC);

  delay (100);
}

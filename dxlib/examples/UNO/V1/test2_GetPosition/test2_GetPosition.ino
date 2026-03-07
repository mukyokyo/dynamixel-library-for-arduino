// Get Present Position
//  Get present position of horn with torque off
//  **Assuming DX/AX/MX Series
//  1.select [SOFT]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE at 115200 bps

#include <dxlib.h>
#include <dxmemmap.h>

#include "avr_uno_softserial.h"

#define TARGET_ID 1
#define BAUDRATE  57143

DXLIB dxif((DXLIB::PDXHost_ConfParam)&param);

void setup() {
  // set terminal baudrate
  Serial.begin (115200);

  // set dxlib baudrate
  dxif.begin (BAUDRATE, 50);

  // Torque off
  dxif.WriteByteData (TARGET_ID, ADDRESS_TORQUE_ENABLE, 0, NULL);
}

void loop() {
  uint16_t ppos;

  // Get present position
  dxif.ReadWordData (TARGET_ID, ADDRESS_PRESENT_POSITION, &ppos, NULL);
  Serial.println (ppos, DEC);

  delay (100);
}

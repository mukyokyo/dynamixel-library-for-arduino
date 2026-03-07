// Flash LED
//  Flashing built-in LED
//  **Assuming X?330/X?430/X?540
//  **When selecting software serial, baudrate must be less than 57600bps.
//  1.Switch SW1 to [SOFT]
//  2.compile & download
//  3.If software serial, switch SW1 to [SOFT], if hardware serial, switch SW1 to [HARD].

#include <dx2lib.h>
#include <dxmemmap.h>
//#include "avr_uno_hardserial.h"
#include "avr_uno_softserial.h"

#define TARGET_ID 1
#define BAUDRATE 57600

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

void setup() {
  // set dxlib baudrate
  dxif.begin(BAUDRATE, 50);
}

void loop() {
  static uint8_t led = 0;
  dxif.WriteByteData(TARGET_ID, ADDRESS_X_LED_RED, led, NULL);
  led ^= 1;
  delay(500);
}

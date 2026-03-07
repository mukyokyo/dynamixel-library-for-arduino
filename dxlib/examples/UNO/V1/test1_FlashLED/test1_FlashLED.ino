
// Flash LED
//  Flashing built-in LED
//  **Assuming DX/AX/MX Series
//  **When selecting software serial, the baud rate must be less than 57600 bps.
//  1.Switch SW1 to [SOFT]
//  2.compile & download
//  3.If software serial, switch SW1 to [SOFT], if hardware serial, switch SW1 to [HARD].

#include <dxlib.h>
#include <dxmemmap.h>

// Activate one of the following two #include lines.
//#include "avr_uno_hardserial.h"
#include "avr_uno_softserial.h"

#define TARGET_ID 1
#define BAUDRATE  57143

DXLIB dxif((DXLIB::PDXHost_ConfParam)&param);

void setup() {
  // set dxlib baudrate
  dxif.begin (BAUDRATE, 50);
}

void loop() {
  static uint8_t led = 0;
  dxif.WriteByteData (TARGET_ID, ADDRESS_LED, led, NULL);
  led ^= 1;
  delay (500);
}

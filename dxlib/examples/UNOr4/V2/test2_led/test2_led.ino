// LED flashing
//  Make the built-in LED blink at regular intervals.
//  1.Select soft/hard serial header
//  2.Switch SW1 to [HARD] or [SOFT]
//  3.compile & download

// **note
//  Assuming X?330/X?430/X?540
//  When selecting hardware serial, baudrate must be less than 2000000bps. (tx only 8Mbps)
//  When selecting software serial, baudrate must be less than 115200bps. (tx only 5Mbps)

#include <dx2lib.h>
#include <dxmemmap.h>

// Activate one of the following two #include lines.
#include "uno_r4_hardserial.h"
//#include "uno_r4_softserial.h"

#define TARGET_ID 1
#define BAUDRATE  57600

// Instantiate DX2LIB
DX2LIB dx2 ((DX2LIB::TDXHost_ConfParam *)&param);

void setup() {
  // set baudrate & timeout
  dx2.begin (BAUDRATE, 50);
}

void loop() {
  uint8_t led;
  // read LED state
  if (dx2.ReadByteData (TARGET_ID, ADDRESS_X_LED_RED, &led))
    // invert LED state
    dx2.WriteByteData (TARGET_ID, ADDRESS_X_LED_RED, led ^ 1);
  delay (500);
}

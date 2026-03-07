// Set Goal Position
//  Move position to the full range
//  **Assuming DX/AX/MX Series
//  **When selecting software serial, baudrate must be less than 57600bps.
//  1.Switch SW1 to [SOFT]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE at 115200 bps

#include <dxlib.h>
#include <dxmemmap.h>

// Activate one of the following two #include lines.
//#include "avr_uno_hardserial.h"
#include "avr_uno_softserial.h"

#define TARGET_ID 1
#define BAUDRATE  57143

DXLIB dxif((DXLIB::PDXHost_ConfParam)&param);

typedef struct {
  int16_t minpos, maxpos;
} Tmaxminpos;

Tmaxminpos maxminpos;
int16_t pos = 0;

void setup() {
  // set terminal baudrate
  Serial.begin (115200);

  // set dxlib baudrate
  dxif.begin (BAUDRATE, 50);

  // Get min/max position limit
  if (dxif.ReadBlockData (TARGET_ID, ADDRESS_CW_ANGLE_LIMIT, (uint8_t *)&maxminpos, sizeof (Tmaxminpos), NULL)) {
    if (dxif.ReadWordData (TARGET_ID, ADDRESS_PRESENT_POSITION, (uint16_t *)&pos, NULL)) {
      Serial.print ("max pos=");
      Serial.print (maxminpos.minpos);
      Serial.print (" min pos=");
      Serial.print (maxminpos.maxpos);
      Serial.print (" presens pos=");
      Serial.println (pos);
    } else Serial.println ("Failed to ReadWordData");
  } else Serial.println ("Failed to ReadBlockData");
}

void loop() {
  static int16_t dir = 10;

  // Set goal position
  if (dxif.WriteWordData (TARGET_ID, ADDRESS_GOAL_POSITION, pos, NULL)) {
    Serial.print (pos);
    Serial.print ("   \r");
  }

  // Update target value
  pos += dir;
  if (pos >= maxminpos.maxpos) {
    pos = maxminpos.maxpos;
    dir *= -1;
  } else if (pos <= maxminpos.minpos) {
    pos = maxminpos.minpos;
    dir *= -1;
  };

  delay (10);
}

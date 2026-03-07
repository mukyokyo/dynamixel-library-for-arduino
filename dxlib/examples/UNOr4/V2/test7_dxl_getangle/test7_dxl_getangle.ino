// Get present angle with intuitive function
//  Use the intuitive function to obtain the angle in degrees
//  **Hide most model-dependent functionality and operate using IDs and physical values.
//  **When selecting hardware serial, baudrate must be less than 2000000bps.
//  1.Switch SW1 to [HARD]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE

#include "uno_r4_hardserial.h"
#include <dx2memmap.h>

#define TARGET_ID 1
#define BAUDRATE 57600

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

// Number of axes found
int num = 0;
// Array holding the found axis
uint8_t ids[253];

extern "C" {
  int _write(int fd, char *ptr, int len) {
    (void)fd;
    return Serial.write(ptr, len);
  }
}

void setup() {
  // set terminal baudrate
  Serial.begin();

  delay(1000);
  // set dxlib baudrate
  dxif.begin(BAUDRATE, 50);

  // Scan devices (Required)
  num = dxif.ScanDevices(ids);
  // Print the scan results
  dxif.PrintDevicesList(&printf);

  // Disable all DXL torques
  dxif.SetTorqueEnablesEquival(ids, num, false);
  delay(500);
}

void loop() {
  double ang[20];

  if (num > 20) num = 20;

  // Get present angle
  if (dxif.GetPresentAngles(ids, ang, num)) {
    for (int i = 0; i < num; i++)
      printf("[%d] %7d ", ids[i], (int)(ang[i] * 10));
    printf("\r\n");
  } else {
    printf("error\r\n");
  }

  delay(100);
}
// Set present angles & velocities with intuitive function
//  Set each DXL to an individual operating mode and simultaneously command both speed and angle.
//  **Hide most model-dependent functionality and operate using IDs and physical values.
//  **Be careful, as we are rewriting the DXL operating mode.
//  **When selecting hardware serial, baudrate must be less than 2000000bps.
//  1.Switch SW1 to [HARD]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE

#include "uno_r4_hardserial.h"
#include <dx2memmap.h>

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

  // Set the operating mode for each ID
  dxif.SetOperatingMode(1, 1);  // velocity
  dxif.SetOperatingMode(2, 1);  // velocity
  dxif.SetOperatingMode(3, 3);  // position
  dxif.SetOperatingMode(4, 3);  // position
  dxif.SetOperatingMode(5, 3);  // position
  dxif.SetOperatingMode(6, 3);  // position

  // Enable all DXL torques
  dxif.SetTorqueEnablesEquival(ids, num, true);
  delay(500);
}

// For now, prepare tables for the 6 axes' velocity and angle in several patterns.
// Only the first and second axes are velocity, while the other axes are angle.
const double pos_ang[][6] = {
  {    0.0,    0.0,    0.0,    0.0,    0.0,    0.0 },
  {  180.0, -180.0,   45.0,   45.0,   45.0,   45.0 },
  {  180.0, -180.0,  -45.0,  -45.0,  -45.0,  -45.0 },
  {  180.0, -180.0,   90.0,   90.0,   90.0,   90.0 },
  { -180.0,  180.0,  -90.0,  -90.0,  -90.0,  -90.0 },
  { -180.0,  180.0,  180.0,  180.0,  180.0,  180.0 },
  { -180.0,  180.0, -180.0, -180.0, -180.0, -180.0 },
};

void loop() {
  uint32_t t;
  static uint32_t next_t = 0;
  static int ind = 0;
  static bool ten = true;
  static uint32_t n = 0;

  t = millis();
  if (t > next_t) {
    printf("pattern=%d\n", ind);
    // Batch commands according to DXL operating mode
    dxif.SetMultipleGoalValues(ids, pos_ang[ind], num, 1.0);
    if (++ind >= sizeof(pos_ang) / sizeof(pos_ang[0])) ind = 0;
    next_t = t + 1000;
  }
  dxif.SetLED(ids[n++ % num], false);
  dxif.SetLED(ids[n % num], true);
  if (Serial.available() > 0) {
    Serial.read();
    ten = !ten;
    printf("torque en=%d\n", ten);
    dxif.SetTorqueEnablesEquival(ids, num, ten);
  }
  delay(50);
}
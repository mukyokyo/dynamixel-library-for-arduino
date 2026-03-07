// Set current with intuitive function
//  Perform position feedback control using the current control mode on the Arduino itself.
//  That alone wasn't enough, so I tried using two DXLs for bilateral control.
//  **Hide most model-dependent functionality and operate using IDs and physical values.
//  **Assuming there are two DXLs (ID=1 and ID=2) capable of current control (adjustments made assuming XC330-T181 here).
//  **Be careful, as we are rewriting the DXL operating mode.
//  **When selecting hardware serial, baudrate must be less than 2000000bps.
//  1.Switch SW1 to [HARD]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE

#include "uno_r4_hardserial.h"
#include "pid.hpp"
#include <dx2memmap.h>

#define BAUDRATE 57600
#define cyctime_ms 20

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);
// PID contol
CPID pid;

// Target IDs are 1 and 2
uint8_t ids[] = { 1, 2 };

// PID gain for controlling angle via current control (Do not use const)
// The gain value is pretty arbitrary.
CPID::TPIDParam gain[2] = {
  // P    I    D    F    Control cycle[s]    min[mA] max[mA]
  { 2.0, 3.0, 0.1, 0.0, cyctime_ms / 1000.0, -500.0, 500.0, },
  { 2.0, 3.0, 0.1, 0.0, cyctime_ms / 1000.0, -500.0, 500.0, }
};

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
  dxif.ScanDevices(NULL);
  // Print the scan results
  dxif.PrintDevicesList(&printf);

  // Switch to current control mode
  if (dxif.SetOperatingMode(ids[0], 0) && dxif.SetOperatingMode(ids[1], 0)) {
    printf("It is probably controllable.\r\n");
    // Enable all DXL torques
    dxif.SetTorqueEnablesEquival(ids, 2, true);
  } else {
    printf("Control is impossible.\r\n");
    for(;;) delay(1000);
  }
  delay(500);
}

void loop() {
  uint32_t t = millis();
  static uint32_t next_t = 0;
  double pang[2], gcur[2];
  static bool ten = true;

  // Wait until the specified cycle time
  if (t > next_t) {
    // Get present angles
    if (dxif.GetPresentAngles(ids, pang, 2)) {
      // Calculation to set the deviation between the angles to zero
      gcur[0] = pid.calc(&gain[0], pang[1] - pang[0], 0.0);
      gcur[1] = pid.calc(&gain[1], pang[0] - pang[1], 0.0);
      // Set currents
      dxif.SetGoalCurrents(ids, gcur, 2);
      // Monitor
      printf("%6.1f,%6.1f,%6.1f,%6.1f\r\n", pang[0], pang[1], gcur[0], gcur[1]);
    }
    next_t = t + cyctime_ms;
  }
  if (Serial.available() > 0) {
    Serial.read();
    ten = !ten;
    printf("torque en=%d\r\n", ten);
    dxif.SetTorqueEnablesEquival(ids, 2, ten);
  }
}
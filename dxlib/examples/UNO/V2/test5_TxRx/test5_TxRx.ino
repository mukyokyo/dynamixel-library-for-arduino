// Direct packet tx/rx
//  Generate packets by specifying ID, instruction number, and parameters.
//  **Assuming MX/X?430/X?540
//  1.Switch SW1 to [SOFT]
//  2.compile & download
//  3.Open the serial monitor of the Arduino IDE at 115200bps

#include "avr_uno_softserial.h"

#define TARGET_ID 1
#define BAUDRATE 57600

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

void setup() {
  Serial.begin(115200);

  dxif.begin(BAUDRATE, 50);
}

void loop() {
  static uint8_t led = 0;

  uint8_t buf[20], rid, err;
  uint16_t len;

  // Tx Ping instruction (no parameter)
  if (dxif.TxPacket(TARGET_ID, 0x01, NULL, 0)) {
    Serial.println("Tx ping OK");
    // Rx Status packet
    if (dxif.RxPacket(buf, sizeof(buf), &rid, &len, &err)) {
      Serial.println("Rx ping OK");
      for (uint16_t i = 0; i < len; i++) {
        Serial.print(buf[i], HEX);
        Serial.print(" ");
      }
    } else Serial.println("Rx NG");
  } else Serial.println("Tx NG");
  Serial.println("");

  // Tx Write instruction
  *(uint16_t *)&buf[0] = 65;
  buf[2] = led;
  if (dxif.TxPacket(TARGET_ID, 0x03, buf, 3)) {
    Serial.println("Tx write OK");
    // Rx Status packet
    if (dxif.RxPacket(buf, sizeof(buf), &rid, &len, &err)) {
      Serial.println("Rx write OK");
      for (uint16_t i = 0; i < len; i++) {
        Serial.print(buf[i], HEX);
        Serial.print(" ");
      }
      led ^= 1;
    } else Serial.println("Rx NG");
  } else Serial.println("Tx NG");
  Serial.println("");

  // Tx Write2 instruction
  *(uint16_t *)&buf[0] = 224;
  buf[2] = 0xff;
  buf[3] = 0xff;
  buf[4] = 0xfd;
  buf[5] = 0xff;
  buf[6] = 0xff;
  buf[7] = 0xfd;
  buf[8] = 0xff;
  buf[9] = 0xff;
  buf[10] = 0xfd;
  if (dxif.TxPacket(TARGET_ID, 0x03, buf, 11)) {
    Serial.println("Tx write2 OK");
    // Rx Status packet
    if (dxif.RxPacket(buf, sizeof(buf), &rid, &len, &err)) {
      Serial.println("Rx write2 OK");
      for (uint16_t i = 0; i < len; i++) {
        Serial.print(buf[i], HEX);
        Serial.print(" ");
      }
    } else Serial.println("Rx NG");
  } else Serial.println("Tx NG");
  Serial.println("");

  // Tx Read instruction
  *(uint16_t *)&buf[0] = 224;
  *(uint16_t *)&buf[2] = 9;
  if (dxif.TxPacket(TARGET_ID, 0x02, buf, 4)) {
    Serial.println("Tx read OK");
    // Rx Status packet
    if (dxif.RxPacket(buf, sizeof(buf), &rid, &len, &err)) {
      Serial.println("Rx read OK");
      for (uint16_t i = 0; i < len; i++) {
        Serial.print(buf[i], HEX);
        Serial.print(" ");
      }
    } else Serial.println("Rx NG");
  } else Serial.println("Tx NG");
  Serial.println("");

  delay(1000);
}
// Configuration
//  Change IDs and operating mode, and perform simple operations.
//  **Assuming X?330/X?430/X?540/P/PH/Y Series
//  1.Switch SW1 to [HARD]
//  2.compile & download
//  3.Open the serial monitors

#include "uno_r4_hardserial.h"
#include <whoareyou.h>

#define _ADDR_ID (7)

#define MAX(a, b) ((a) > (b) ? a : b)
#define MIN(a, b) ((a) < (b) ? a : b)

DX2LIB dxif((DX2LIB::PDXHost_ConfParam)&param);

void setup() {
  Serial.begin();
  dxif.begin(57600, 50);
}

// Discard of incoming data
void DiscardRxData(void) {
  delay(100);
  do {
    if (Serial.available() > 0) Serial.read();
  } while (Serial.available() > 0);
}

// Numeric input only
bool InputNum(int32_t *num) {
  int i = 0;
  char buf[100], ch = 0;

  while ((ch != 0x0d) && (ch != 0x0a)) {
    if (Serial.available() > 0) {
      ch = Serial.read();
      if (ch >= '0' and ch <= '9') {
        Serial.print(ch);
        buf[i++] = ch;
      }
    }
  }
  buf[i] = 0;
  DiscardRxData();
  *num = atoi(buf);
  return (strlen(buf));
}

// Yes or No
bool YorN(void) {
  char ch;
  while (Serial.available() == 0)
    ;
  ch = toupper(Serial.read());
  DiscardRxData();
  return (ch == 'Y');
}

// Persistent search for devices with specified ID
bool DeviceExists(uint8_t id) {
  bool detected = false;
  for (int i = 0; i < 5; i++) {
    if ((detected = dxif.Ping(id, NULL))) break;
  }
  return detected;
}

void loop() {
  uint8_t num;
  static TAlarmStatus stat[253];
  uint16_t modelno;
  static uint8_t id = 1;
  uint8_t b, err;
  int32_t d;
  PModelInfo pminfo = NULL;
  PMajorItemsAddr paddr = NULL;

  if (Serial.available() > 0) {
    char ch = Serial.read();
    DiscardRxData();

    Serial.print(ch);
    switch (ch) {
      case 's':  // scan
        Serial.print("\r\nStat scan\r\n");
        for (uint8_t i = 0; i <= 252; i++) {
          if (Serial.available() > 0) {
            DiscardRxData();
            break;
          }
          if (dxif.Ping(i, NULL)) {
            if (dxif.ReadWordData(i, 0, &modelno, NULL)) {
              PModelInfo p = whoareyou(modelno);
              Serial.println("");
              Serial.print(i);
              Serial.print(":");
              Serial.println(p->name);
            } else Serial.print(" ");
          } else Serial.print(".");
        }
        break;

      case 'S':  // scan (use ping2)
        Serial.print("\r\nStat scan\r\n");
        num = 253;
        num = dxif.Ping2(&num, stat);
        for (uint8_t i = 0; i <= num; i++) {
          if (dxif.ReadWordData(stat[i].id, 0, &modelno)) {
            PModelInfo p = whoareyou(modelno);
            Serial.print(i);
            Serial.print(":");
            Serial.println(p->name);
          }
        }
        break;

      case 'p':  // ping
        Serial.print("\r\nPing to id=");
        Serial.print(id);
        Serial.print(dxif.Ping(id, NULL) ? " ok" : " ng");
        break;

      case 'R':  // factory reset device
        if (dxif.Ping(id, NULL)) {
          Serial.print("\r\nFactory reset device\r\nAre you sure ?(Y/N)");
          if (YorN()) {
            Serial.print("y\r\n");
            if (!DeviceExists(1) || (id == 1)) {
              if (dxif.Reset(id, &err)) {
                Serial.print("\r\nOK\r\n");
                id = 1;
              } else {
                Serial.print("\r\nNG: $");
                Serial.println(err, HEX);
              }
            } else
              Serial.println("NG: That setting conflicts");
          } else
            Serial.println("n");
        } else
          Serial.println("\r\nCannot fRind device");
        break;
      case 'i':  // change host id
        Serial.print("\r\nChange host id\r\nInput id (0..252) = ");
        if (InputNum(&d))
          if (d >= 0 && d <= 252) id = d;
        break;
      case 'I':
        if (dxif.Ping(id, NULL)) {
          Serial.print("\r\nChange device id\r\nInput id (0..252) = ");
          if (InputNum(&d)) {
            d = MAX(MIN(d, 252), 0);
            if (!DeviceExists(d)) {
              if (dxif.WriteByteData(id, _ADDR_ID, d, &err)) {
                Serial.print("\r\nOK\r\n");
                id = d;
              } else {
                Serial.print("\r\nNG: $");
                Serial.println(err, HEX);
              }
            } else
              Serial.println("NG: That setting conflicts");
          }
        } else
          Serial.println("\r\nCannot find device");
        break;

      case 'O':  // change device operating mode
        if (dxif.Ping(id, NULL)) {
          if (dxif.ReadWordData(id, 0, &modelno, NULL)) {
            pminfo = whoareyou(modelno);
            paddr = itemaddr(modelno);

            if (dxif.ReadByteData(id, paddr->opmode.addr, &b, NULL)) {
              Serial.print("\r\nChange device op mode (current op mode = ");
              Serial.print(b);
              Serial.print(")\r\nInput op mode (0:cur 1:velo 3:pos 4:multiturn 5:multi+cur 16:PWM) = ");
              if (InputNum(&d)) {
                b = MAX(MIN(d, 16), 0);

                if (dxif.WriteByteData(id, paddr->opmode.addr, b, NULL)) {
                  Serial.print("\r\nOK");
                } else {
                  Serial.print("\r\nNG: $");
                  Serial.print(err, HEX);
                }
              }
            }
          }
        } else
          Serial.print("\r\nCannot find device\r\n");
        break;

      case 'E':  // change device torque enable
        Serial.print("\r\n");
        if (dxif.Ping(id, NULL)) {
          if (dxif.ReadWordData(id, 0, &modelno, NULL)) {
            pminfo = whoareyou(modelno);
            paddr = itemaddr(modelno);
            if (dxif.ReadByteData(id, paddr->torque_en.addr, &b, NULL)) {
              b = b ^ 1;
              if (dxif.WriteByteData(id, paddr->torque_en.addr, b, NULL)) {
                Serial.print("Change device torqule ");
                Serial.print(b == 0 ? "Off" : "On");
              }
            }
          } else
            Serial.print("Cannot find device\r\n");
        } else
          Serial.print("Cannot find device\r\n");
        break;

      case 'A':
        printf("\r\n");
        if (dxif.Ping(id, NULL)) {
          if (dxif.ReadWordData(id, 0, &modelno, NULL)) {
            pminfo = whoareyou(modelno);
            paddr = itemaddr(modelno);
            if (dxif.ReadByteData(id, paddr->opmode.addr, &b, NULL)) {
              if ((b == 3) || (b == 4) || (b == 5)) {
                if (dxif.ReadLongData(id, paddr->p_pos.addr, &d, NULL)) {
                  int32_t dblval_flg = abs(pminfo->positionlimit.max / 512);
                  while (Serial.available() == 0) {
                    d += dblval_flg;
                    if (d > pminfo->positionlimit.max) {
                      dblval_flg *= -1;
                      d = pminfo->positionlimit.max;
                    } else if (d < pminfo->positionlimit.min) {
                      dblval_flg *= -1;
                      d = pminfo->positionlimit.min;
                    }
                    dxif.WriteLongData(id, paddr->g_pos.addr, d, NULL);
                    delay(1);
                  }
                }
                DiscardRxData();
              }
              Serial.print("\r\n");
            } else
              Serial.print("NG: op mode is different\r\n");
          }
        } else
          Serial.print("Cannot find device\r\n");
        break;

      case 'V':
        printf("\r\n");
        if (dxif.Ping(id, NULL)) {
          if (dxif.ReadWordData(id, 0, &modelno, NULL)) {
            pminfo = whoareyou(modelno);
            paddr = itemaddr(modelno);
            if (dxif.ReadByteData(id, paddr->opmode.addr, &b, NULL)) {
              if (b == 1) {
                if (dxif.ReadLongData(id, paddr->p_velo.addr, &d, NULL)) {
                  int32_t dblval_flg = 1;
                  while (Serial.available() == 0) {
                    d += dblval_flg;
                    if (d > 1023) {
                      dblval_flg = -1;
                      d = 1023;
                    } else if (d < -1023) {
                      dblval_flg = 1;
                      d = -1023;
                    }
                    dxif.WriteLongData(id, paddr->g_velo.addr, d, NULL);
                    delay(1);
                  }
                }
                DiscardRxData();
              }
              Serial.print("\r\n");
            } else
              Serial.print("NG: op mode is different\r\n");
          }
        } else
          Serial.print("Cannot find device\r\n");
        break;
    }
    Serial.print("\r\nid:");
    Serial.print(id);
    Serial.print(">");
  }
}
// Serial to USB bridge
//  Move position to the full range
//  1.Forcibly replaced with one that includes the Serial library.
//  2.Switch SW1 to [HARD]
//  3.compile & download

// **note
//  1.Resolving serial library issues
//  You need to rewrite the library using the following site as a reference.
//  https://www.hackster.io/chuygen/fix-your-arduino-uno-r4-uart-issues-b52e4a
//  Note that the buffer size in the code for this site is small, so replace it
//  with the files in the included replace folder.
//  The baud rate matches up to 8 Mbps, but SCI data reception errors become
//  frequent around 2 Mbps and above.
//
//  2.Disable USB hardware flow control
//  Calling the Serial.dtr() function disables DTR, enabling communication in
//  most applications that use the serial port.
//  This eliminates the need to activate the COM port's hardware flow control
//  and prevents the UNO R4 from resetting every time the USB cable is plugged
//  or unplugged.
//  As a trade-off, it becomes impossible to automatically trigger a reset when
//  uploading from the Arduino IDE.

uint32_t prevbaud = 57600;

void setup() {
  pinMode(LED_TX, OUTPUT);
  pinMode(LED_RX, OUTPUT);
  digitalWrite(LED_TX, HIGH);
  digitalWrite(LED_RX, HIGH);

  Serial.begin(prevbaud);
  // ignore dtr
  Serial.dtr();

  Serial1.begin(prevbaud);
}

void loop() {
  int l, ll;
  static uint8_t buf[4096];
  static uint32_t tx_t = 0, rx_t = 0;
  uint32_t nowbaud;

  if (millis() > tx_t) digitalWrite(LED_TX, HIGH);
  if (millis() > rx_t) digitalWrite(LED_RX, HIGH);

  while ((l = Serial.available()) > 0) {
    digitalWrite(LED_TX, LOW);
    tx_t = millis() + 20;
    while ((ll = Serial.readBytes(buf, min(sizeof(buf), l))) > 0) {
      Serial1.write(buf, ll);
      l -= ll;
    }
  }

  while ((l = Serial1.available()) > 0) {
    digitalWrite(LED_RX, LOW);
    while ((ll = Serial1.readBytes(buf, min(sizeof(buf), l))) > 0) {
      Serial.write(buf, ll);
      l -= ll;
    }
  }

  if ((nowbaud = Serial.baud()) != prevbaud) {
    Serial1.flush();
    Serial1.begin(nowbaud);
    prevbaud = nowbaud;
  }
}